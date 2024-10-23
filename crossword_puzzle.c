#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Constants
#define SIZE 40
#define CELL_SIZE 18
#define MAX_QUESTIONS 16
#define MAX_WORD_LENGTH 30
#define TIME_LIMIT 1200

// Struct to represent a question in the crossword puzzle
typedef struct
{
    char text[512];               // Full text of the question
    int row;                      // Starting row for the answer
    int column;                   // Starting column for the answer
    char direction;               // 'H' for horizontal, 'V' for vertical
    char answer[MAX_WORD_LENGTH]; // Correct answer
    char hint[256];               // Hint for the question
} Question;

// Enum to represent different game screens
typedef enum
{
    MENU,
    PLAY,
    CREDENTIALS,
    FINISHED
} GameScreen;

// Enum to represent answer status
typedef enum
{
    NONE,
    CORRECT,
    INCORRECT
} AnswerStatus;

// Arrays to represent the grids
char grid1[SIZE][SIZE]; // User's grid
char grid2[SIZE][SIZE] = {
    // Predefined crossword solution
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'F', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'U', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', 'G', 'R', 'O', '-', 'F', 'O', 'R', 'E', 'S', 'T', 'Y', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', 'Y', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'E', 'R', 'M', 'A', 'C', 'U', 'L', 'T', 'U', 'R', 'E', ' ', ' ', 'T', ' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'Y', ' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'U', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', ' ', ' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', 'W', 'E', 'L', 'F', 'A', 'R', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'U', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'L', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'C', 'O', 'M', 'M', 'O', 'N', ' ', 'A', 'G', 'R', 'I', 'C', 'U', 'L', 'T', 'U', 'R', 'A', 'L', ' ', 'P', 'O', 'L', 'I', 'C', 'Y', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'L', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'R', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'L', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'U', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'M', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'R', 'E', 'E', 'N', ' ', 'D', 'E', 'A', 'L', ' ', ' ', ' ', ' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'N', ' ', ' ', ' ', 'B', 'I', 'O', 'F', 'E', 'R', 'T', 'I', 'L', 'I', 'Z', 'E', 'R', ' ', ' ', ' ', ' ', ' ', ' ', 'L', ' ', ' ', ' ', ' ', 'B', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'U', 'S', 'T', 'A', 'I', 'N', 'A', 'B', 'I', 'L', 'I', 'T', 'Y', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'M', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'V', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', 'F', 'F', 'O', 'R', 'D', 'A', 'B', 'L', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'F', 'A', 'R', 'M', ' ', 'T', 'O', ' ', 'F', 'O', 'R', 'K', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'Y', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'O', 'P', ' ', 'R', 'O', 'T', 'A', 'T', 'I', 'O', 'N', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

// Array of questions for the crossword puzzle
Question questions[MAX_QUESTIONS] = {
    {"What is the name of the EU policy that aims to have 25% of agricultural land under organic farming by 2030?", 23, 10, 'H', "_____ ____", "EU initiative for environmental sustainability."},
    {"What does the growing of heirloom and older varieties intend to preserve? The ___ of seeds.", 26, 29, 'V', "____________", "Variety of plant and animal life."},
    {"One of the six EU-wide Green Deal targets is the societal objective to improve animal ___.", 14, 23, 'H', "_______", "Government assistance for those in need."},
    {"What environmental benefit is associated with the use of biofertilizers in sustainable agriculture?", 10, 1, 'V', "_______ _________", "Efforts to lower harmful emissions."},
    {"Organic farming only allows the use of natural substances and aims at avoiding the massive exploitation of natural resources instead of using a more damaging method. What did that method use?", 9, 24, 'V', "_________ _________", "Man-made substances."},
    {"The Farm To Fork Strategy hopes to generate fairer economic returns in the supply chain, so that ultimately the most sustainable food also becomes the most ___.", 33, 10, 'H', "__________", "Priced reasonably."},
    {"What practice combines agricultural and forestry technologies to create more diverse, productive, healthy and sustainable microclimate that stabilizes and improve soil structure?", 9, 15, 'H', "____-_______", "Combines trees with crops/livestock."},
    {"What term describes the strategy to help farmers and other professional pesticide users to practice environmentally friendly pest control?", 14, 16, 'V', "__________ ____ __________", "Holistic pest control."},
    {"What term is used for substances containing live microbes that help in enhancing soil fertility by fixing atmospheric nitrogen, solubilizing phosphorus, or decomposing organic wastes?", 26, 5, 'H', "_____________", "Organic soil enhancer."},
    {"What is another innovative approach when it comes to tackling weed problems, other than poly-cultures?", 39, 9, 'H', "____ _________", "Sequential planting strategy."},
    {"In which domain of activity does the EU Commission intend to assure at least 30% of the budgetary allowance for research and innovation with the aim to achieve the EU Organic farming Action Plan' objectives?", 11, 12, 'V', "___________", "Farming and livestock raising."},
    {"An agricultural practice oriented towards the natural maintenance of the soil with an optimal and ethical design that results in a low-maintenance garden that does not require irrigation or fertilization?", 11, 8, 'H', "____________", "Sustainable farming design."},
    {"The EU's Common Agriculture Policy (CAP) is built around three main goals: economic, environmental and social ___.", 27, 24, 'H', "______________", "Meeting needs without harm."},
    {"What is the policy framework that supports a modern, market-oriented farming sector in the EU?", 19, 0, 'H', "______ ____________ ______", "EU farming guidelines."},
    {"What strategy did the EU use to transition to sustainable systems because of its urgent need to reduce dependency on pesticides and excess fertilization, increase organic farming, improve animal welfare, and reverse biodiversity loss?", 36, 13, 'H', "____ __ ____", "Shortened food supply chain."},
    {"By its policies that help raise standards globally and externalize unsustainable practices, the EU hopes to reduce the environmental and climate footprints, and lead a global transition towards competitive sustainability and one other goal. What is the last target?", 0, 22, 'V', "____ ________", "Access to nutritious food."}};

// Function to initialize the grid with empty spaces
void initialize_grid()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            grid1[i][j] = ' '; // Fill the grid with empty spaces
        }
    }
}

// Function to display the grid on the screen
void display_grid()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            // Draw each cell in the grid
            DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, WHITE);
            DrawRectangleLines(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);

            if (grid1[i][j] == '-')
            {
                // Draw a dash if the cell contains a dash
                DrawText("-", j * CELL_SIZE + 5, i * CELL_SIZE + 2, CELL_SIZE - 4, BLACK);
            }
            else if (grid1[i][j] != ' ')
            {
                // Draw the character if the cell is not empty
                DrawText(TextFormat("%c", grid1[i][j]), j * CELL_SIZE + 5, i * CELL_SIZE + 2, CELL_SIZE - 4, BLACK);
            }
        }
    }
}

// Function to check if the given word matches the answer on the grid
int check_word(char *word, int row, int column, char direction)
{
    int length = strlen(word);
    int correct = 1; // Assume the answer is correct

    if (direction == 'H')
    {
        for (int i = 0; i < length; i++)
        {
            if (grid2[row][column + i] != word[i])
            {
                correct = 0;
                break;
            }
        }
    }
    else if (direction == 'V')
    {
        for (int i = 0; i < length; i++)
        {
            if (grid2[row + i][column] != word[i])
            {
                correct = 0;
                break;
            }
        }
    }

    if (correct && strlen(word) == length)
    {
        return 1; // Return 1 for a correct answer
    }

    return 0; // Return 0 for an incorrect answer
}

// Function to place the word on the grid
void place_word(char *word, int row, int column, char direction)
{
    int length = strlen(word);

    if (direction == 'H')
    {
        for (int i = 0; i < length; i++)
        {
            grid1[row][column + i] = word[i]; // Place the word horizontally
        }
    }
    else if (direction == 'V')
    {
        for (int i = 0; i < length; i++)
        {
            grid1[row + i][column] = word[i]; // Place the word vertically
        }
    }
}

// Function to draw text with word wrapping
void DrawTextWrapped(const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    int length = TextLength(text); // Get the length of the text
    float textOffsetY = 0;         // Initialize the vertical offset for text
    float startX = 0;              // Initialize the horizontal offset for text

    for (int i = 0; i < length; i++) // Loop through each character in the text
    {
        char letter = text[i]; // Get the current character

        if (letter == ' ') // If the character is a space
        {
            startX += MeasureText(" ", fontSize); // Adjust the width for space
            continue;                             // Continue to the next character
        }

        // If word wrapping is enabled and text exceeds the rectangle width
        if (wordWrap && (startX + MeasureText(TextFormat("%c", letter), fontSize)) > rec.width)
        {
            startX = 0;                        // Reset the horizontal offset
            textOffsetY += fontSize + spacing; // Move to the next line
        }

        if (letter == '\n') // If the character is a newline
        {
            startX = 0;                        // Reset the horizontal offset
            textOffsetY += fontSize + spacing; // Move to the next line
            continue;                          // Continue to the next character
        }

        // Draw the current character
        DrawText(TextFormat("%c", letter), rec.x + startX, rec.y + textOffsetY, fontSize, tint);
        startX += MeasureText(TextFormat("%c", letter), fontSize); // Update the horizontal offset
    }
}

int main(void)
{
    // Initialize the window
    InitWindow(1280, 720, "Crossword Puzzle");
    SetWindowSize(1280, 720);

    SetTargetFPS(60); // Set the target frames per second

    initialize_grid(); // Initialize the crossword grid

    int current_question = 0;          // Index for the current question
    char input[MAX_WORD_LENGTH] = "";  // Buffer for user input
    int input_index = 0;               // Index for user input buffer
    time_t start_time = time(NULL);    // Start time for the game
    time_t last_status_time = 0;       // Last time the status was updated
    GameScreen screen = MENU;          // Initial game screen
    AnswerStatus answer_status = NONE; // Initial answer status

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the screen with white background

        if (screen == MENU) // Menu screen
        {
            float titleSize = 60;
            float titleWidth = MeasureText("Crossword Puzzle", titleSize);

            float buttonSize = 40;
            float buttonHeight = buttonSize + 10;

            float buttonWidthPlay = MeasureText("Play", buttonSize) + 20;
            float buttonWidthCredentials = MeasureText("Credits", buttonSize) + 20;

            // Center the title on the screen
            float startX = (GetScreenWidth() - titleWidth) / 2;
            float startY = (GetScreenHeight() - (titleSize + buttonHeight * 2 + 20)) / 2;

            // Draw the title
            DrawText("Crossword Puzzle", startX, startY, titleSize, BLACK);

            // Draw the Play button
            DrawRectangle(GetScreenWidth() / 2 - buttonWidthPlay / 2,
                          startY + titleSize + 20,
                          buttonWidthPlay, buttonHeight, LIGHTGRAY);
            DrawRectangleLines(GetScreenWidth() / 2 - buttonWidthPlay / 2,
                               startY + titleSize + 20,
                               buttonWidthPlay, buttonHeight, DARKGRAY);
            DrawText("Play",
                     GetScreenWidth() / 2 - MeasureText("Play", buttonSize) / 2,
                     startY + titleSize + 25, buttonSize, DARKBLUE);

            // Draw the Credits button
            DrawRectangle(GetScreenWidth() / 2 - buttonWidthCredentials / 2,
                          startY + titleSize + 20 + buttonHeight + 10,
                          buttonWidthCredentials, buttonHeight, LIGHTGRAY);
            DrawRectangleLines(GetScreenWidth() / 2 - buttonWidthCredentials / 2,
                               startY + titleSize + 20 + buttonHeight + 10,
                               buttonWidthCredentials, buttonHeight, DARKGRAY);
            DrawText("Credits",
                     GetScreenWidth() / 2 - MeasureText("Credits", buttonSize) / 2,
                     startY + titleSize + 25 + buttonHeight + 10, buttonSize, DARKBLUE);

            Vector2 mousePoint = GetMousePosition(); // Get the current mouse position

            // Check if the mouse is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Check if the Play button is clicked
                if (CheckCollisionPointRec(mousePoint,
                                           (Rectangle){GetScreenWidth() / 2 - buttonWidthPlay / 2,
                                                       startY + titleSize + 20,
                                                       buttonWidthPlay,
                                                       buttonHeight}))
                {
                    screen = PLAY;           // Change to play screen
                    start_time = time(NULL); // Reset the start time
                }
                // Check if the Credits button is clicked
                else if (CheckCollisionPointRec(mousePoint,
                                                (Rectangle){GetScreenWidth() / 2 - buttonWidthCredentials / 2,
                                                            startY + titleSize + 20 + buttonHeight + 10,
                                                            buttonWidthCredentials,
                                                            buttonHeight}))
                {
                    screen = CREDENTIALS; // Change to credits screen
                }
            }
        }
        else if (screen == PLAY) // Play screen
        {
            int key = GetCharPressed(); // Get the pressed key

            // Loop to process all pressed keys
            while (key > 0)
            {
                // Check if the key is an alphabet or space and within input buffer limit
                if ((isalpha(key) || key == ' ') && (input_index < (MAX_WORD_LENGTH - 1)))
                {
                    input[input_index] = (char)toupper(key); // Add the character to the input buffer
                    input[input_index + 1] = '\0';           // Null-terminate the string
                    input_index++;
                }

                key = GetCharPressed(); // Get the next pressed key
            }

            // Check if the dash key is pressed
            if (IsKeyPressed(KEY_MINUS) && (input_index < (MAX_WORD_LENGTH - 1)))
            {
                input[input_index] = '-';      // Add a dash to the input buffer
                input[input_index + 1] = '\0'; // Null-terminate the string
                input_index++;
            }

            // Check if the backspace key is pressed
            if (IsKeyPressed(KEY_BACKSPACE) && input_index > 0)
            {
                input_index--;             // Decrease the input index
                input[input_index] = '\0'; // Null-terminate the string
            }

            // Check if the enter key is pressed
            if (IsKeyPressed(KEY_ENTER))
            {
                // Check if the input word matches the current question's answer
                if (check_word(input, questions[current_question].row,
                               questions[current_question].column,
                               questions[current_question].direction))
                {
                    // Check if the answer length matches
                    if (strlen(input) == strlen(questions[current_question].answer))
                    {
                        answer_status = CORRECT; // Set the status to correct
                        place_word(input, questions[current_question].row,
                                   questions[current_question].column,
                                   questions[current_question].direction); // Place the word on the grid
                        current_question++;                                // Move to the next question
                        input_index = 0;                                   // Reset the input index
                        input[0] = '\0';                                   // Clear the input buffer
                        last_status_time = time(NULL);                     // Update the status time

                        // Check if all questions are answered
                        if (current_question >= MAX_QUESTIONS)
                        {
                            screen = FINISHED; // Change to finished screen
                        }
                    }
                    else
                    {
                        answer_status = INCORRECT;     // Set the status to incorrect
                        input_index = 0;               // Reset the input index
                        input[0] = '\0';               // Clear the input buffer
                        last_status_time = time(NULL); // Update the status time
                    }
                }
                else
                {
                    answer_status = INCORRECT;     // Set the status to incorrect
                    input_index = 0;               // Reset the input index
                    input[0] = '\0';               // Clear the input buffer
                    last_status_time = time(NULL); // Update the status time
                }
            }

            display_grid(); // Display the crossword grid

            // Draw the question and input area
            DrawRectangle(720, 0, 560, 720, WHITE);

            DrawText("Question:", 730, 20, 20, BLACK);
            DrawTextWrapped(questions[current_question].text, (Rectangle){730, 50, 460, 200}, 20, 5, true, BLACK);

            DrawText(TextFormat("Answer format: %s", questions[current_question].answer), 730, 260, 20, DARKGRAY);

            DrawText(TextFormat("Hint: %s", questions[current_question].hint), 730, 290, 20, DARKGRAY);

            // Calculate the remaining time
            time_t current_time = time(NULL);
            int time_left = TIME_LIMIT - difftime(current_time, start_time);

            // Check if time is up
            if (time_left <= 0)
            {
                current_question++;      // Move to the next question
                input_index = 0;         // Reset the input index
                input[0] = '\0';         // Clear the input buffer
                start_time = time(NULL); // Reset the start time
                answer_status = NONE;    // Reset the answer status

                // Check if all questions are answered
                if (current_question >= MAX_QUESTIONS)
                {
                    screen = FINISHED; // Change to finished screen
                }
            }

            // Display the remaining time
            DrawText(TextFormat("Time left: %02d:%02d", time_left / 60, time_left % 60), 730, 320, 20, RED);

            // Display the user's input
            DrawText(TextFormat("Your answer: %s_", input), 730, 350, 20, BLACK);

            // Display the status of the last answer
            if (answer_status == CORRECT && (current_time - last_status_time < 1))
            {
                DrawText("Correct!", 730, 400, 30, GREEN);
            }
            else if (answer_status == INCORRECT && (current_time - last_status_time < 1))
            {
                DrawText("Incorrect!", 730, 400, 30, RED);
            }
        }
        else if (screen == CREDENTIALS) // Credits screen
        {
            int yOffset = 100;   // Starting point for the credit text
            int lineHeight = 40; // Height of each line of text

            // Display the credits
            DrawText("EU Programme Erasmus+",
                     GetScreenWidth() / 2 - MeasureText("EU Programme Erasmus+", 40) / 2,
                     yOffset, 40, BLACK);
            yOffset += lineHeight + 10; // Add the line height and a space between lines

            DrawText("Project title: 'Gender, Digitalization, Green: Ensuring a Sustainable Future for all in Europe'",
                     GetScreenWidth() / 2 - MeasureText("Project title: 'Gender, Digitalization, Green: Ensuring a Sustainable Future for all in Europe'", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Project No.: 2023-1-RO01-KA220-HED-000154433",
                     GetScreenWidth() / 2 - MeasureText("Project No.: 2023-1-RO01-KA220-HED-000154433", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Module 4: Green Agriculture",
                     GetScreenWidth() / 2 - MeasureText("Module 4: Green Agriculture", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Game made by:",
                     GetScreenWidth() / 2 - MeasureText("Game made by:", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Vlad Cirloban (vlad.cirloban@stud.acs.upb.ro)",
                     GetScreenWidth() / 2 - MeasureText("Vlad Cirloban (vlad.cirloban@stud.acs.upb.ro)", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Cosmin Cosmulete (ion.cosmulete@stud.acs.upb.ro)",
                     GetScreenWidth() / 2 - MeasureText("Cosmin Cosmulete (ion.cosmulete@stud.acs.upb.ro)", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Diana Ghetea (andra_diana.ghetea@stud.acs.upb.ro)",
                     GetScreenWidth() / 2 - MeasureText("Diana Ghetea (andra_diana.ghetea@stud.acs.upb.ro)", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Rares Sova (ioan_rares.sova@stud.acs.upb.ro)",
                     GetScreenWidth() / 2 - MeasureText("Rares Sova (ioan_rares.sova@stud.acs.upb.ro)", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            DrawText("Teachers: Prof. M. Caramihai & D. Chis",
                     GetScreenWidth() / 2 - MeasureText("Teachers: Prof. M. Caramihai & D. Chis", 20) / 2,
                     yOffset, 20, BLACK);
            yOffset += lineHeight;

            // Prompt to return to menu
            DrawText("Press SPACE to return to menu",
                     GetScreenWidth() / 2 - MeasureText("Press SPACE to return to menu", 30) / 2,
                     GetScreenHeight() - 100, 30, DARKGRAY);

            // Check if the space key is pressed
            if (IsKeyPressed(KEY_SPACE))
            {
                screen = MENU; // Change to menu screen
            }
        }
        else if (screen == FINISHED) // Finished screen
        {
            // Display the congratulations message
            DrawText("Congratulations!",
                     GetScreenWidth() / 2 - MeasureText("Congratulations!", 60) / 2,
                     200, 60, BLACK);

            DrawText("You've completed the crossword puzzle!",
                     GetScreenWidth() / 2 - MeasureText("You've completed the crossword puzzle!", 40) / 2,
                     300, 40, DARKGRAY);

            // Prompt to return to menu
            DrawText("Press SPACE to return to menu",
                     GetScreenWidth() / 2 - MeasureText("Press SPACE to return to menu", 30) / 2,
                     GetScreenHeight() - 100, 30, DARKGRAY);

            // Check if the space key is pressed
            if (IsKeyPressed(KEY_SPACE))
            {
                screen = MENU;        // Change to menu screen
                current_question = 0; // Reset the current question index
                input_index = 0;      // Reset the input index
                input[0] = '\0';      // Clear the input buffer
                initialize_grid();    // Reinitialize the grid
            }
        }

        EndDrawing(); // End the drawing
    }

    CloseWindow(); // Close the window

    return 0;
}