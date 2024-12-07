#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <limits>
#include <string>
#include <unordered_map>
#include<vector>
#include "Code_lib/ExpressionTree.h"
#include<cstdlib>
#include<unordered_map>
#include<ctime>
using namespace std;
// Define color constants
/*
1. Space-separated Tokens:
Leave a space between every number, variable, operator, and parenthesis.
Example:
Correct: A + B * ( C - D )
Incorrect: A+B*(C-D)

2. Allowed Operators:
The following operators are supported:
+ (addition), - (subtraction), * (multiplication), / (division), % (modulus), and ^ (exponentiation).

3. Parentheses:
Use parentheses for grouping expressions to define precedence explicitly in an infix expression.
Ensure parentheses are balanced (each opening parenthesis ( has a corresponding closing parenthesis )).

4. Variable Names:
Variable names must start with a letter and may include letters and digits.
Example: x, y1, var2, AB are valid, but 1x and x-y are not.

5. Numbers:
You can use integers (e.g., 2, -5) or decimals (e.g., 3.14, -0.75).
Note: Two decimal points in a number are not allowed. For example, 3.14.5 is invalid.


6. Negative Numbers:
Negative numbers should be entered as -5 or -3.14 with the minus sign directly preceding the number.

7. Operand Requirement for Binary Operators:
Don’t miss operands for binary operators (operators that require two operands, like +, -, *, /).
For example, an expression like A + or * B is invalid because it lacks a second operand.

8. Expression Type:
Do not mix expression types. If you choose a type, make sure the expression matches that type, or the program will produce an error.

9. Handling Variables:
If the expression contains variables, you must enter correct numeric values for all variables.
*/

 int X[5][7] = {  {1,0,0,0,0,0,1},
    {0,1,0,0,0,1,0},
    {0,0,1,0,1,0,0},
    {0,1,0,0,0,1,0},
    {1,0,0,0,0,0,1}
};
  int P[5][7] = {  {1,1,1,1,1,0,0},
    {1,0,0,0,0,1,0},
    {1,1,1,1,1,0,0},
    {1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0}
};
  int S[5][7] = {  {1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0},
    {1,1,1,1,1,1,1},
    {0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1}
};
 int I[5][7] ={   {1,1,1,1,1,1,1},
    {0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0},
    {1,1,1,1,1,1,1}
};
   int N[5][7] ={ {1,0,0,0,0,0,1},
    {1,1,0,0,0,0,1},
    {1,0,1,0,0,0,1},
    {1,0,0,1,0,0,1},
    {1,0,0,0,1,1,1}
};
  int C[5][7] ={  {1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0},
    {1,1,1,1,1,1,1}
};

   int O[5][7] ={ {1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1}
};

   int V[5][7] =
       { {1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1},
    {0,1,0,0,0,1,0},
    {0,0,1,1,1,0,0}};

      int T[5][7] =
      { {1,1,1,1,1,1,1},
    {0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0} };

     int E[5][7] =
     { {1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0},
    {1,1,1,1,1,0,0},
    {1,0,0,0,0,0,0},
    {1,1,1,1,1,1,1}
    };
   int R[5][7] =
       {{1,1,1,1,1,0,0},
    {1,0,0,0,0,1,0},
    {1,1,1,1,1,0,0},
    {1,0,0,0,1,0,0},
    {1,0,0,0,0,1,0}
};

// Function to render a character block at (x, y)
const SDL_Color RED = {255, 0, 0, 255};
const SDL_Color GREEN = {0, 255, 0, 255};
const SDL_Color BLUE = {0, 0, 255, 255};
const SDL_Color WHITE = {255, 255, 255, 255};

struct Node {
    int x, y, width, height;
    SDL_Color borderColor;
    std::vector<int> children; // Indices of children nodes
};
// Function to render a node
void renderNode(SDL_Renderer* renderer, const Node& node) {
    // Draw the border
    SDL_SetRenderDrawColor(renderer, node.borderColor.r, node.borderColor.g, node.borderColor.b, node.borderColor.a);
    SDL_Rect borderRect = { node.x, node.y, node.width, node.height };
    SDL_RenderDrawRect(renderer, &borderRect);


    // Fill the node with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect fillRect = { node.x + 1, node.y + 1, node.width - 2, node.height - 2 };
    SDL_RenderFillRect(renderer, &fillRect);
}
// Function to render edges
void renderEdges(SDL_Renderer* renderer, const std::vector<Node>& nodes) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White edges
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (int childIndex : nodes[i].children) {
            if (childIndex < nodes.size()) { // Safety check for child index
                SDL_RenderDrawLine(renderer,
                                   nodes[i].x + nodes[i].width / 2, nodes[i].y + nodes[i].height,
                                   nodes[childIndex].x + nodes[childIndex].width / 2, nodes[childIndex].y);
            }
        }
    }
}
void showLoadingScreen(SDL_Renderer* renderer, ExpressionTree::TreeNode* root) {
    int nodeWidth = 50, nodeHeight = 50;
    int startX = 550, startY = 75, spacingX = 80, spacingY = 80;
    int maxNodes = 31;


    // Create nodes and tree structure
    std::vector<Node> nodes;
    nodes.push_back({ startX, startY, nodeWidth, nodeHeight, { 255, 0, 0, 255 }, {} }); // Root node


    int currentIndex = 0;
    int nodeCount = 1;
    std::vector<int> nodesToDraw = { 0 }; // Indices of nodes to render


    // Main loop
    bool running = true;
    SDL_Event event;


    bool growing = true;
    int levelStart = 1;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }


        // Clear screen with black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        // Render edges and nodes
        renderEdges(renderer, std::vector<Node>(nodes.begin(), nodes.begin() + nodesToDraw.size()));
        for (size_t i = 0; i < nodesToDraw.size(); ++i) {
            renderNode(renderer, nodes[nodesToDraw[i]]);
        }


        // Present the renderer
        SDL_RenderPresent(renderer);


        // Animate growth or decay
        if (growing) {
            // Add nodes level by level
            SDL_Delay(230);
            if (nodeCount < maxNodes) {
                int parentIndex = currentIndex;


                for (int j = -1; j <= 1; j += 2) { // Two children per parent, left and right
                    if (nodeCount >= maxNodes) break;


                    int childX = nodes[parentIndex].x + j * spacingX;
                    int childY = nodes[parentIndex].y + spacingY;


                    nodes.push_back({ childX, childY, nodeWidth, nodeHeight, { static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), 255 }, {} });
                    nodes[parentIndex].children.push_back(nodes.size() - 1);
                    nodesToDraw.push_back(nodes.size() - 1);
                    nodeCount++;
                }


                currentIndex++;
            } else {
                growing = false; // Switch to decay after full growth
                levelStart = nodesToDraw.size();
            }
        } else {
            // Remove nodes level by level
            SDL_Delay(150);
            if (levelStart > 0) {
                levelStart /= 2; // Move to the previous level
                nodesToDraw.resize(levelStart);
            } else {
                running = false; // End simulation
            }



        }
    }
}
//
void renderCharacter(SDL_Renderer* renderer, int character[5][7], int x, int y, SDL_Color color) {
    const int BLOCK_SIZE = 10; // Size of each block
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 7; ++col) {
            if (character[row][col] == 1) {
                SDL_Rect block = {x + col * BLOCK_SIZE, y + row * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }
}
// A simple utility function to render text on the screen using SDL_ttf
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
       //cerr << "Unable to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture) {
      // cerr << "Unable to create text texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}
void showCalculatorGraphic(SDL_Renderer* renderer, TTF_Font* font) {
    // Colors
    SDL_Color bgColor = {30, 30, 30, 255};        // Background color
    SDL_Color screenColor = {50, 50, 50, 255};   // Calculator screen color
    SDL_Color buttonColor = {100, 100, 100, 255}; // Button color
    SDL_Color textColor = {255, 255, 255, 255};  // Text color

    // Draw the calculator body
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);

    // Draw calculator screen
    SDL_Rect screenRect = {400, 50, 300, 100};
    SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);
    SDL_RenderFillRect(renderer, &screenRect);

    renderText(renderer, font, "Loading...", 500, 90, textColor);

    // Draw calculator buttons
    int buttonWidth = 70, buttonHeight = 70;
    int startX = 400, startY = 200;
    int buttonSpacing = 20;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 3; ++col) {
            SDL_Rect buttonRect = {
                startX + col * (buttonWidth + buttonSpacing),
                startY + row * (buttonHeight + buttonSpacing),
                buttonWidth,
                buttonHeight};

            SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
            SDL_RenderFillRect(renderer, &buttonRect);

            // Add numbers/text to buttons
            int buttonNumber = row * 3 + col + 1;
            std::string buttonText = (buttonNumber <= 9) ? std::to_string(buttonNumber) : (row == 3 && col == 0) ? "0" : (row == 3 && col == 1) ? "+" : "=";
            renderText(renderer, font, buttonText, buttonRect.x + 25, buttonRect.y + 15, textColor);
        }
    }

    // Show the loading bar
    int barWidth = 300, barHeight = 20;
    SDL_Rect barBackground = {400, 550, barWidth, barHeight};
    SDL_Rect barForeground = {400, 550, 0, barHeight};

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // Bar background
    SDL_RenderFillRect(renderer, &barBackground);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Bar foreground

    for (int i = 0; i <= barWidth; i += 10) {
        barForeground.w = i;
        SDL_RenderFillRect(renderer, &barForeground);
        SDL_RenderPresent(renderer);
        SDL_Delay(50); // Simulate loading time
    }

    SDL_Delay(500); // Hold the screen briefly before continuing
}
std::unordered_map<std::string, double> getVariableValues(SDL_Renderer* renderer, TTF_Font* font, ExpressionTree::TreeNode* root) {
    std::unordered_map<std::string, double> variableValues;

    // Lambda function to traverse the tree and collect variables
    std::function<void(ExpressionTree::TreeNode*)> collectVariables = [&](ExpressionTree::TreeNode* node) {
        if (!node) return;
        if (ExpressionTree::isVariable(node->value)) {
            if (variableValues.find(node->value) == variableValues.end()) {
                std::string userInput;
                bool waitingForInput = true;

                while (waitingForInput) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);

                    renderText(renderer, font, "Enter the value for variable '" + node->value + "':", 50, 250, {255, 255, 255, 255});
                    renderText(renderer, font, userInput, 50, 300, {255, 255, 255, 255});
                    SDL_RenderPresent(renderer);

                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_TEXTINPUT) {
                            userInput += event.text.text; // Append typed characters
                        } else if (event.type == SDL_KEYDOWN) {
                            if (event.key.keysym.sym == SDLK_RETURN) {
                                try {
                                    double value = std::stod(userInput); // Convert input to double
                                    variableValues[node->value] = value;
                                    waitingForInput = false;
                                } catch (...) {
                                    // Handle invalid input
                                    userInput.clear();
                                    renderText(renderer, font, "Invalid input. Please enter a numeric value.", 50, 350, {255, 0, 0, 255});
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(2000);
                                }
                            } else if (event.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()) {
                                userInput.pop_back(); // Remove the last character
                            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                                waitingForInput = false; // Exit input
                            }
                        }
                    }
                    SDL_Delay(16);
                }
            }
        }
        collectVariables(node->left);
        collectVariables(node->right);
    };

    collectVariables(root);
    return variableValues;
}
void runExpressionConverter(SDL_Renderer* renderer, TTF_Font* font) {
    ExpressionTree exprTree;
    std::string input;
    int choice = 0;
    bool quit = false, enteringExpression = false;
    SDL_Event event;

    SDL_StartTextInput();

    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!enteringExpression) {
            renderText(renderer, font, "Choose input type (1: Infix, 2: Prefix, 3: Postfix):", 50, 50, {255, 0, 0, 255});
            renderText(renderer, font, input, 50, 100, {255, 255, 255, 255});
        } else {
            renderText(renderer, font, "Enter the expression:", 50, 100, {255, 0, 0, 255});
            renderText(renderer, font, input, 50, 150, {255, 255, 255, 255});
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                if (!enteringExpression) {
                    try {
                        choice = std::stoi(input);
                        input.clear();
                        enteringExpression = true;
                    } catch (...) {
                        input.clear();
                    }
                } else {
                    try {
                        ExpressionTree::TreeNode* root = nullptr;
                        switch (choice) {
                            case 1: root = exprTree.buildTreeFromInfix(input); break;
                            case 2: root = exprTree.buildTreeFromPrefix(exprTree.tokenize(input)); break;
                            case 3: root = exprTree.buildTreeFromPostfix(exprTree.tokenize(input)); break;
                            default: throw std::invalid_argument("Invalid choice");
                        }

                        std::string infix = exprTree.inorder(root);
                        std::string prefix = exprTree.preorder(root);
                        std::string postfix = exprTree.postorder(root);

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        showLoadingScreen(renderer,root);
                        renderText(renderer, font, "Infix: " + infix, 50, 100, {255, 0, 0, 255});
                        renderText(renderer, font, "Prefix: " + prefix, 50, 150, {0, 0, 255, 255});
                        renderText(renderer, font, "Postfix: " + postfix, 50, 200, {0, 255, 0, 255});
                        SDL_RenderPresent(renderer);
                        SDL_Delay(2000);

                        // Ask if the user wants to evaluate the expression
                        std::string userResponse;
                        bool waitingForResponse = true;
                        while (waitingForResponse) {

                            renderText(renderer, font, "Do you want to evaluate this expression? (y/n):", 50, 300, {255, 255, 255, 255});
                            renderText(renderer, font, userResponse, 50, 350, {255, 255, 255, 255});
                            SDL_RenderPresent(renderer);

                            SDL_PollEvent(&event);
                            if (event.type == SDL_TEXTINPUT) {
                                userResponse += event.text.text;
                            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                                if (userResponse == "y" || userResponse == "Y") {
                                    auto variableValues = getVariableValues(renderer, font, root);
                                    double result = exprTree.evaluate(root, variableValues);
                                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                    SDL_RenderClear(renderer);
                                    showCalculatorGraphic(renderer, font);
                                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                    SDL_RenderClear(renderer);
                                    renderText(renderer, font, "Result: " + std::to_string(result), 50, 350, {255, 255, 255, 255});
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(3000);
                                }
                                waitingForResponse = false;
                            }
                        }

                        // Ask if the user wants to continue
                        userResponse.clear();
                        waitingForResponse = true;
                        while (waitingForResponse) {
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);

                            renderText(renderer, font, "Do you want to convert another expression? (y/n):", 50, 400, {255, 255, 255, 255});
                            renderText(renderer, font, userResponse, 50, 450, {255, 255, 255, 255});
                            SDL_RenderPresent(renderer);

                            SDL_PollEvent(&event);
                            if (event.type == SDL_TEXTINPUT) {
                                userResponse += event.text.text;
                            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                                if (userResponse == "n" || userResponse == "N") {
                                    quit = true;
                                    waitingForResponse = false;
                                } else if (userResponse == "y" || userResponse == "Y") {
                                    input.clear();
                                    enteringExpression = false;
                                    waitingForResponse = false;
                                }
                            }
                        }
                    } catch (const std::exception& e) {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        renderText(renderer, font, std::string("Error: ") + e.what(), 50, 300, {255, 0, 0, 255});
                        SDL_RenderPresent(renderer);
                        SDL_Delay(3000);
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                input += event.text.text;
            }
        }

        SDL_Delay(16);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderText(renderer, font, "Thank you for using the Expression Converter!", 50, 300, WHITE);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_StopTextInput();
}
int main(int argc, char* argv[]) {
        // Initialize SDL and TTF
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
            return 1;
        }
        if (TTF_Init() < 0) {
            std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        SDL_Window* window = SDL_CreateWindow(
            "Expression Converter",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            1200, 800,
            SDL_WINDOW_SHOWN
        );

        if (!window) {
            //std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            //std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        TTF_Font* font = TTF_OpenFont("C:/Users/ASUS/Desktop/untitled2/BrownieStencil-8O8MJ.ttf", 24); // Specify your TTF font path
        if (!font) {
            std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        SDL_Color white = {255, 255, 255, 255};
        SDL_Color blue = {0, 0, 255, 255};

        bool running = true;
        SDL_Event event;
        bool splashScreen = true; // Tracks if we are in the splash screen phase

        // Splash Screen Logic
        while (running && splashScreen) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    splashScreen = false; // Exit the splash screen
                }
            }

            // Render the splash screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            int (*expression[])[5][7] = {&E, &X, &P, &R, &E, &S, &S, &I, &O, &N};
            int numExpression = sizeof(expression) / sizeof(expression[0]);

            int (*converter[])[5][7] = {&C, &O, &N, &V, &E, &R, &T, &E, &R};
            int numConverter = sizeof(converter) / sizeof(converter[0]);

            int xStartExpression = 60;
            int yStartExpression = 200;
            int xStartConverter = 100;
            int yStartConverter = 350;
            for (int i = 0; i < numExpression; ++i) {
                renderCharacter(renderer, *expression[i], xStartExpression + i * (7 * 15 + 10), yStartExpression, RED);
            }

            // Render "Converter"
            for (int i = 0; i < numConverter; ++i) {
                renderCharacter(renderer, *converter[i], xStartConverter + i * (7 * 15 + 10), yStartConverter, BLUE);
            }

            //SDL_RenderPresent(renderer);
            // SDL_Delay(100);
            renderText(renderer, font, "Press Enter to Start", 400, 550, white);
            SDL_RenderPresent(renderer);
        }

        // Run the Expression Converter logic
        if (running) {
            runExpressionConverter(renderer, font); // This calls your function
        }

        // Cleanup
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

