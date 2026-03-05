class Neuron {
    constructor(numInputs) {
        this.weights = Array.from(
            { length: numInputs },
            () => Math.random() * 2 - 1
        )
        this.bias = Math.random() * 2 - 1
    }

    activate(inputs) {
        let sum = this.bias
        for (let i = 0; i < inputs.length; i++) {
            sum += inputs[i] * this.weights[i]
        }
        return Math.tanh(sum)
    }

    mutate(rate = 0.1) {
        for (let i = 0; i < this.weights.length; i++) {
            if (Math.random() < rate) {
                this.weights[i] += (Math.random() * 2 - 1) * 0.2
            }
        }
        if (Math.random() < rate) {
            this.bias += (Math.random() * 2 - 1) * 0.2
        }
    }

    clone() {
        const n = new Neuron(this.weights.length)
        n.weights = [...this.weights]
        n.bias = this.bias
        return n
    }
}

class NeuralNetwork {
    constructor(inputSize, hiddenSize) {
        this.inputSize = inputSize
        this.hiddenSize = hiddenSize

        // capa oculta
        this.hidden = []
        for (let i = 0; i < hiddenSize; i++) {
            this.hidden.push(new Neuron(inputSize))
        }

        // capa de salida (1 neurona)
        this.output = new Neuron(hiddenSize)
    }

    predict(inputs) {
        // inputs → hidden
        const hiddenOutputs = []
        for (let neuron of this.hidden) {
            hiddenOutputs.push(neuron.activate(inputs))
        }

        // hidden → output
        return this.output.activate(hiddenOutputs)
    }

    mutate(rate = 0.1) {
        for (let neuron of this.hidden) {
            neuron.mutate(rate)
        }
        this.output.mutate(rate)
    }

    clone() {
        const nn = new NeuralNetwork(this.inputSize, this.hiddenSize)

        nn.hidden = this.hidden.map(n => n.clone())
        nn.output = this.output.clone()

        return nn
    }
}


class Ground {
    constructor(x, y, width, height, color = "#555", index = 0) {
        this.x = x
        this.y = y
        this.width = width
        this.height = height
        this.color = color
        this.index = index
        this.obstacles = []
    }

    draw(ctx) {
        ctx.fillStyle = this.color
        ctx.fillRect(this.x, this.y, this.width, this.height)
        for (let obs of this.obstacles) {
            obs.draw(ctx, this.x, this.y)
        }
    }

    update(speed, floorArray, firstSafeSegments = 2) {
        this.x -= speed

        if (this.x + this.width < 0) {
            let rightestSegment = Math.max(...floorArray.map(seg => seg.x))
            this.x = rightestSegment + this.width

            if (this.index >= firstSafeSegments) {
                this.generateObstacles()
            } else {
                this.obstacles = []
            }
        }
    }

    generateObstacles() {
        this.obstacles = [] 
        let numObs = Math.floor(Math.random() * 2)
        for (let i = 0; i < numObs; i++) {
            let obsWidth = 30
            let obsHeight = 30
            let xRel = Math.random() * (this.width - obsWidth)
            let yRel = 0 
            this.obstacles.push(new Obstacle(xRel, yRel, obsWidth, obsHeight, "green"))
        }
    }
}

class Obstacle {
    constructor(xRel, yRel, width, height, color = "green") {
        this.xRel = xRel
        this.yRel = yRel
        this.width = width
        this.height = height
        this.color = color
    }

    draw(ctx, segmentX, segmentY) {
        ctx.fillStyle = this.color
        ctx.fillRect(segmentX + this.xRel, segmentY - this.height, this.width, this.height)
    }
}

class Circle {
    constructor(x, y, radius, color) {
        this.x = x
        this.y = y
        this.radius = radius
        this.color = color

		this.alive = true
		this.fitness = 0

        // física
        this.vy = 0
        this.onGround = false

		// red neuronal
        this.brain = new NeuralNetwork(4, 6)
    }

    jump() {
        this.vy = -12
    }

    getNextObstacle(floor) {
        let closest = null
        let minDist = Infinity

        for (let segment of floor) {
            for (let obs of segment.obstacles) {
                const obsX = segment.x + obs.xRel
                const dist = obsX - this.x

                if (dist > 0 && dist < minDist) {
                    minDist = dist
                    closest = {
                        x: obsX,
                        height: obs.height,
                        groundY: segment.y
                    }
                }
            }
        }

        return { obstacle: closest, distance: minDist }
    }

    update(canvas, floor, gravity) {
        // gravedad
		if (!this.alive)
			return
        this.vy += gravity
        this.y += this.vy

        // detectar suelo bajo el círculo
        let groundY = canvas.height
        for (let seg of floor) {
            if (this.x >= seg.x && this.x <= seg.x + seg.width) {
                groundY = seg.y
                break
            }
        }

        if (this.y + this.radius >= groundY) {
            this.y = groundY - this.radius
            this.vy = 0
            this.onGround = true
        } else {
            this.onGround = false
        }

        // decidir si saltar
        const { obstacle, distance } = this.getNextObstacle(floor)

        const distNorm = obstacle ? distance / canvas.width : 1
        const heightNorm = obstacle ? obstacle.height / canvas.height : 0
        const vyNorm = this.vy / 15
        const onGroundNorm = this.onGround ? 1 : 0

        const inputs = [distNorm, heightNorm, vyNorm, onGroundNorm]
        const decision = this.brain.predict(inputs)

        if (decision > 0 && this.onGround) {
            this.jump()
        }
			// colisión con obstáculos
		for (let segment of floor) {
				for (let obs of segment.obstacles) {
				const obsX = segment.x + obs.xRel
				const obsY = segment.y - obs.height

				if (
						this.x + this.radius > obsX &&
						this.x - this.radius < obsX + obs.width &&
						this.y + this.radius > obsY
				) {
						// MUERTE
						this.alive = false
						return
				}
				}
		}

		this.fitness += 1
    }

    draw(ctx) {
		ctx.beginPath()
		ctx.drawImage(bird, this.x, this.y - 55, 120, 120)
		ctx.fillStyle = 'red'
        //ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2)
		ctx.fill()
	}
}

class Population {
		constructor(size, startX, startY) {
				this.circles = []
				this.startX = startX
				this.startY = startY
				for (let i = 0; i < size; i++) {
						const color = `hsl(${Math.random() * 360}, 70%, 50%)`
						const c = new Circle(startX, startY, 30, color)
						this.circles.push(c)
				}
				this.generation = 1
		}

		allDead() {
				return this.circles.every(c => !c.alive)
		}

		update(canvas, floor, gravity) {
				for (let c of this.circles) {
						c.update(canvas, floor, gravity)
				}
				
				if (this.allDead()) {
						this.evolve()
						resetGame()
				}
		}

		evolve() {
				const newCircles = []

				this.circles.sort((a, b) => b.fitness - a.fitness)
				const top = this.circles.slice(0, 10)

				while (newCircles.length < this.circles.length) {
				// escoger un padre al azar entre los top
				const parent = top[Math.floor(Math.random() * top.length)]

				// clonar su cerebro
				const childBrain = parent.brain.clone()

				// aplicar mutación
				childBrain.mutate(0.3) // tasa de mutación

				// crear un nuevo círculo con el mismo tamaño y color aleatorio
				const color = `hsl(${Math.random() * 360}, 70%, 50%)`
				const child = new Circle(this.startX, this.startY, parent.radius, color)
				child.brain = childBrain

				newCircles.push(child)
				}
				this.circles = newCircles
				this.generation += 1

		}
		draw(ctx) {
				for (let c of this.circles)
						if (c.alive) 
								c.draw(ctx)
		}
		getBestCircle() {
				// Ordena por fitness descendente
				return this.circles.reduce((best, c) => {
						return (best === null || c.fitness > best.fitness) ? c : best
				}, null)
		}

}
function drawNode(x, y, radius = 10, color = "white") {
    ctx.beginPath()
    ctx.arc(x, y, radius, 0, Math.PI * 2)
    ctx.fillStyle = color
    ctx.fill()
    ctx.strokeStyle = "black"
    ctx.stroke()
}

function drawConnection(x1, y1, x2, y2, weight) {
    const alpha = Math.min(Math.abs(weight), 1)
    ctx.strokeStyle = weight > 0 ? `rgba(0,255,0,${alpha})` : `rgba(255,0,0,${alpha})`
    ctx.lineWidth = Math.abs(weight) * 2
    ctx.beginPath()
    ctx.moveTo(x1, y1)
    ctx.lineTo(x2, y2)
    ctx.stroke()
}

function drawNetwork(brain) {
    const inputX = panelX
    const hiddenX = panelX + 70
    const outputX = panelX + 140

    // posiciones verticales
    const inputYStart = panelY
    const hiddenYStart = panelY
    const outputY = panelY + 50

    // dibujar conexiones input → hidden
    for (let i = 0; i < brain.inputSize; i++) {
        const yInput = inputYStart + i * nodeSpacing
        for (let j = 0; j < brain.hidden.length; j++) {
            const yHidden = hiddenYStart + j * nodeSpacing
            drawConnection(xInput = inputX, yInput, xHidden = hiddenX, yHidden, brain.hidden[j].weights[i])
        }
    }

    // dibujar conexiones hidden → output
    for (let j = 0; j < brain.hidden.length; j++) {
        const yHidden = hiddenYStart + j * nodeSpacing
        drawConnection(hiddenX, yHidden, outputX, outputY, brain.output.weights[j])
    }

    // dibujar nodos
    for (let i = 0; i < brain.inputSize; i++) {
        const yInput = inputYStart + i * nodeSpacing
        drawNode(inputX, yInput)
    }
    for (let j = 0; j < brain.hidden.length; j++) {
        const yHidden = hiddenYStart + j * nodeSpacing
        drawNode(hiddenX, yHidden)
    }
    drawNode(outputX, outputY, 12, "orange") // output en naranja
}



function resetGame() {

    for (let i = 0; i < floor.length; i++) {
        floor[i].x = i * floor[i].width
        floor[i].obstacles = []

        // generar obstáculos solo si no es un segmento seguro
        if (i >= 4) { // primeros 2 segmentos vacíos
            floor[i].generateObstacles()
        }
    }
}

const canvas = document.getElementById("canvas")
const ctx = canvas.getContext("2d")
const gravity = 0.5

canvas.width = window.innerWidth
canvas.height = window.innerHeight

const groundHeight = 60
const segmentWidth = 200
const numSegments = Math.ceil(canvas.width / segmentWidth) + 2


const floor = []
for (let i = 0; i < numSegments; i++) {
    floor.push(new Ground(i * segmentWidth, canvas.height - groundHeight, segmentWidth, groundHeight, "#555", i))
}

const worldSpeed = 10
const population = new Population(100, 200, 100)
const panelX = canvas.width - 200  // esquina derecha
const panelY = 50
const nodeSpacing = 30
let bird = document.getElementById('bird')
function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height)

    for (let seg of floor) {
        seg.update(worldSpeed, floor)
        seg.draw(ctx)
    }
    
    population.update(canvas, floor, gravity)
    population.draw(ctx)

    const best = population.getBestCircle()
    if (best)
		drawNetwork(best.brain)

    ctx.font = "28px Aria"
    ctx.fillText(`Generacion: ${population.generation}`, panelX, 250)
    ctx.fillText(`Vivos: ${population.circles.filter(c => c.alive).length}`, panelX, 280)
    requestAnimationFrame(animate)
}

animate()

