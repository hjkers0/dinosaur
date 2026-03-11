Some time ago I did this implementation of NN for the game of Dinosaur (chrome) that only learns when to jump.

Recently I encounter the Raylib library that is normaly used for games and I wanted to give it a try in C.

# Implementation on JS
This was the very first code that I did, also the easiest one (simpliest also). This 'game' is using the HTML _canvas_ for displaying
all the logic.
- Neuron {Weights, Bias} with the functions _activate_, _mutate_, _clone_.
- NeuralNetwork {inputSize, hiddenSize, hiddenNeurons, outputNeuron } with the funcions _predict_, _mutate_, _clone_
- Population { size, startX, startY, generation } with funtions to display it on the canvas.

The Population is the set of 'dinosaurs' (circles) that keep bouncing randomly at first but rapidly learns how to avoid the obtacules.

The NeuralNetwork that I will use have 4 inputs:
- Distance to next obstacle
- Height of the next obstacle
- Y velocity of object (dinosaur)
- Is on the ground?

I know that this parameters may be not the better for an proper solution but I just wanted to try with something simple at first and did not change it
for the final version. Maybe on the future...

And that is it, the rest of the code is just the drawing on the canvas.

# Implementation on C
For the C implementation I use the [*Raylib*](https://www.raylib.com/) library for displaying the game. This is a library mostly used on games so I thought
that this is a correct use for it.

In this implementation I used some header files just to make a bit prettier but surely it can be done in just 1 file.

The files are:
- nn.h / nn.c -> Contains all the code related to the NeuralNetwork.
- obstacle.h / obstacle.h -> Code related for managing the obstacles that appears on the game.
- main.c -> Main file with the integration with Raylib and the core functionality of the game.


# Implementation on WASM
This last days wanted to show the demo of the game on [github-pages](https://hjkers0.github.io/dinosaur/) and wanted to try to compile the game to WASM.
For this implementation I used the [Emscripten](https://emscripten.org/) tool for compiling the C code into WASM.

On the Makefile there are some flags for using the Emscripten tool in order to compile using ASYNCFY. In that way I did not have to
change anything of the original C version of the game in order to make it work on web. All that is well explained on the Raylib [docs](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))
