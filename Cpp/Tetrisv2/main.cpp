#define ENABLE_ALL_CLASSES 1
#define ENABLE_ALL_STRUCTS 1
#define ENABLE_ALL_FUNCTIONS 1

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <chrono>
#include <thread>
#include <cmath>

//built-in variables
const int MAX_WIDTH = 12; // 2 side borders included
const int MAX_HEIGHT = 13; // floor and ceil borders included
const char TETRIS_MAP[MAX_WIDTH][MAX_HEIGHT] = {0}; // unused

//specified chars
const char borderChar = '#';
const char removeLineChar = '=';
const char whitespaceChar = ' ';
//global app variables
bool exitProgram = false;
bool getInput(char *c);

#if ENABLE_ALL_CLASSES
class Debug
{
    public:
    std::vector<std::string> logs;
    Debug()
    {
        logs.clear();
    }
    void Log(std::string sLog)
    {
        logs.push_back(sLog);
    }
    void PrintLogs()
    {
        for(unsigned int i = 0; i < logs.size(); i++)
        {
            std::cout << "Log " << i << ": " << logs[i] << "\n";
        }
    }
    void PrintLastLog()
    {
        if(logs.size() > 0)
            std::cout << "Last log[" << logs.size() << "]: " << logs[logs.size()-1] << "\n";
        else
            std::cout << "There are no avaiable logs to display!\n";
    }
    ~Debug()
    {
        PrintLogs();
    }
};
#endif

#if ENABLE_ALL_STRUCTS
struct Vector2Range
{
    int xMin, xMax, yMin, yMax;
};
#endif

#if ENABLE_ALL_FUNCTIONS

char *getShape(bool random, int type)
{
    char *nShape = new char [8];
    int choosenShape = type;
    if(random)
        choosenShape = (int)((rand() + 3) * (rand() - 1))%7+1;
    switch(choosenShape)
    {
    case 1: // G
        {
            char cShape[8] =
            {
                ' ', ' ',
                'G', 'G',
                'G', 'G',
                ' ', ' '
            };
            nShape = cShape;
        }
        break;
    case 2: // L
        {
            char cShape[8] =
            {
                ' ', ' ',
                'L', ' ',
                'L', ' ',
                'L', 'L',
            };
            nShape = cShape;
        }
        break;
    case 3: // J
        {
            char cShape[8] =
            {
                ' ', ' ',
                ' ', 'J',
                ' ', 'J',
                'J', 'J',
            };
            nShape = cShape;
        }
        break;
    case 4: // I
        {
            char cShape[8] =
            {
                'I', ' ',
                'I', ' ',
                'I', ' ',
                'I', ' '
            };
            nShape = cShape;
        }
        break;
    case 5: // T
        {
            char cShape[8] =
            {
                ' ', ' ',
                'T', ' ',
                'T', 'T',
                'T', ' ',
            };
            nShape = cShape;
        }
        break;
    case 6: // S
        {
            char cShape[8] =
            {
                ' ', ' ',
                'S', ' ',
                'S', 'S',
                ' ', 'S',
            };
            nShape = cShape;
        }
        break;
    case 7: // Z
        {
            char cShape[8] =
            {
                ' ', ' ',
                ' ', 'Z',
                'Z', 'Z',
                'Z', ' ',
            };
            nShape = cShape;
        }
        break;
    case 0:
        std::cout << "Element does not exist!\n";
        break;
    default:
        std::cout << "Wrong type of shape! Type must be between 1-7 (included)\n";
        break;
    }
    return nShape;
}

void createMapBorders(char tMap[MAX_HEIGHT][MAX_WIDTH])
{
    for(int i = 0; i < MAX_HEIGHT-1; i++)
    {
        tMap[i][0] = tMap[i][MAX_WIDTH-1] = borderChar;
    }
    for(int i = 0; i < MAX_WIDTH; i++)
    {
        tMap[MAX_HEIGHT-1][i] = borderChar;
    }
}
void clearMap(char tMap[MAX_HEIGHT][MAX_WIDTH])
{
    for(int x = 0; x < MAX_HEIGHT; x++) // just check if bottom is free or removed high
    {
        for(int y = 0; y < MAX_WIDTH; y++)
        {
            tMap[x][y] = whitespaceChar;
        }
    }
}
// Get Input
bool getInput(char *c)
{
    if (kbhit())
    {
         *c = getch();
         return true; // Key Was Hit
    }
	return false; // No keys were pressed
}
#endif // ENABLE_ALL_FUNCTIONS

int main()
{
    Debug debug = Debug();
    std::clock_t start;
    double duration;
    start = std::clock();

    std::cout << "Hello world of Tetris!\n";
    std::cout << "Type 'play' to start game: \n";
    std::string command;
    while(std::cin >> command)
    {
        if(command == "play")
            break;
        std::cout << "Want to exit game? [yes]/[no]\n";
        std::cin >> command;
        if(command == "yes")
            return 0;
        if(command == "no")
        {
            system("cls");
            std::cout << "Type 'play' to start game: \n";
        }
        else
        {
            system("cls");
            std::cout << "Unknown command. Type 'play' to start game: \n";
        }
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("cls");

    for(int counter = 3; counter > 0; counter--)
    {
        std::cout << "Game starts in " << counter;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        system("cls");
    }
    double startTimer = std::clock();
    char tetrisMap[MAX_HEIGHT][MAX_WIDTH];
    clearMap(tetrisMap);
    createMapBorders(tetrisMap);
    int points = 0;
    bool waitForNewElement = false;
    bool gameOver = false;
    char key = ' ';
    Vector2Range currentShapePos;// x min, x max, y min, y max
    char currentShapeInfo[8] = {' '};

    while(true)
    {
        // system("cls");
        int delayMS = 1000;
        if(exitProgram)
        {
            debug.Log("Program exited!");
            break;
        }
        if(gameOver)
        {
            debug.Log("Game over!");
            break;
        }
        if(!waitForNewElement)
        {
            debug.Log("Creating new shape.");
            // debug.PrintLastLog();
            char *newShape = getShape(true, 0);
            int mid = MAX_WIDTH/2;
            currentShapePos.yMin = mid; currentShapePos.yMax = mid+1;
            currentShapePos.xMin = 0; currentShapePos.xMax = 3;
            for(int x = 0; x < 8; x++) // just check if bottom is free or removed high
            {
                bool g = false, o = false;
                if(newShape[x] != ' ')
                    g = (tetrisMap[x][mid] != whitespaceChar || tetrisMap[x][mid+1] != whitespaceChar) ? true : false;
                if(newShape[x+1] != ' ')
                    o = (tetrisMap[x][mid] != whitespaceChar || tetrisMap[x][mid+1] != whitespaceChar) ? true : false;
                gameOver = (g || o) ? true : gameOver;
                int xMid = x/2;
                tetrisMap[xMid][mid] = currentShapeInfo[x] = newShape[x];
                x++;
                tetrisMap[xMid][mid+1] = currentShapeInfo[x] = newShape[x];
            }
            waitForNewElement = true;
        }

        debug.Log("Rotating and moving shapes.");
        // debug.PrintLastLog();
        while (!getInput(&key)){}
        bool cannotMove = false;
        switch(key)
        {
        case KEY_LEFT:
            for(int x = currentShapePos.xMin; x <= currentShapePos.xMax; x++)
            {
                for(int y = currentShapePos.yMin; y <= currentShapePos.yMax; y++)
                {
                    if(tetrisMap[x][y-1] != ' ' && tetrisMap[x][y] != ' ' && y - 1 < currentShapePos.yMin)
                    {
                        std::cout << "left\n";
                        cannotMove = true;
                        break;
                    }
                }
                if(cannotMove)
                    break;
            }
            if(cannotMove)
                break;
            for(int x = currentShapePos.xMin; x <= currentShapePos.xMax; x++)
            {
                for(int y = currentShapePos.yMin; y <= currentShapePos.yMax; y++)
                {
                    if(tetrisMap[x][y] != whitespaceChar && tetrisMap[x][y] != borderChar)
                        tetrisMap[x][y-1] = tetrisMap[x][y];
                    tetrisMap[x][y] = tetrisMap[x][y] == borderChar ? borderChar : whitespaceChar;
                }
            }
            currentShapePos.yMin--; currentShapePos.yMax--;
            break;
        case KEY_RIGHT:
            for(int x = currentShapePos.xMin; x <= currentShapePos.xMax; x++)
            {
                for(int y = currentShapePos.yMax; y >= currentShapePos.yMin; y--)
                {
                    if(tetrisMap[x][y+1] != ' ' && tetrisMap[x][y] != ' ' && y + 1 > currentShapePos.yMax)
                    {
                        std::cout << "right\n";
                        cannotMove = true;
                        break;
                    }
                }
                if(cannotMove)
                    break;
            }
            if(cannotMove)
                break;
            for(int x = currentShapePos.xMin; x <= currentShapePos.xMax; x++)
            {
                for(int y = currentShapePos.yMax; y >= currentShapePos.yMin; y--)
                {
                    if(tetrisMap[x][y] != whitespaceChar && tetrisMap[x][y]!= borderChar)
                        tetrisMap[x][y+1] = tetrisMap[x][y];
                    tetrisMap[x][y] = tetrisMap[x][y] == borderChar ? borderChar : whitespaceChar;
                }
            }
            currentShapePos.yMin++; currentShapePos.yMax++;
            break;
        case KEY_UP:
            switch(currentShapePos.yMax - currentShapePos.yMin + 1)
            {
            case 2:
                {
                    Vector2Range cSP = currentShapePos;
                    int x5Pos = cSP.xMax, y5Pos = cSP.yMax;
                    std::cout << "up2start\n";
                    if (x5Pos - 2 < 1 || x5Pos + 1 > MAX_HEIGHT - 1 || y5Pos < 1 || y5Pos + 2 >= MAX_WIDTH)
                    {
                        std::cout << "up " << x5Pos << " h w " << y5Pos << std::endl;
                        std::cout << "up2border\n";
                        cannotMove = true;
                        break;
                    }
                    for(int y = y5Pos + 2, i = cSP.xMax; y > y5Pos; y--, i--)
                    {
                        for(int x = x5Pos - 2, j = cSP.yMin; x < x5Pos; x++, j++)
                        {
                            std::cout <<  x << " x y" << y << "tetrisxy " << tetrisMap[x][y] << y+1 << std::endl;
                            if(tetrisMap[x][y] != ' ' && tetrisMap[i][j] != ' ' && y + 1 >= y5Pos + 2)
                            {
                                std::cout << "up\n";
                                cannotMove = true;
                                break;
                            }
                        }
                        if(cannotMove)
                            break;
                    }
                    std::cout << "up2start2\n";
                    if(cannotMove)
                        break;

                    char cTab[8] =
                    {
                        currentShapeInfo[6],    // 3|0|6
                        currentShapeInfo[4],    // 2|0|4
                        currentShapeInfo[2],    // 1|0|2
                        currentShapeInfo[0],    // 0|0|0
                        currentShapeInfo[7],    // 3|1|7
                        currentShapeInfo[5],    // 2|1|5
                        currentShapeInfo[3],    // 1|1|3
                        currentShapeInfo[1]     // 0|1|1
                    };
                    for(int x = cSP.xMin; x <= cSP.xMax; x++)
                    {
                        for(int y = cSP.yMin; y <= cSP.yMax; y++)
                        {
                            if(x != 0 && x != MAX_HEIGHT - 1 && y != 0 && y != MAX_WIDTH - 1)
                                tetrisMap[x][y] = whitespaceChar ? whitespaceChar : tetrisMap[x][y];
                        }
                    }
                    std::cout << "upend1\n";

                    cSP.xMin = x5Pos - 2; cSP.xMax = x5Pos - 1;
                    cSP.yMin = y5Pos - 1; cSP.yMax = y5Pos + 2;
                    for(int x = cSP.xMin, i = 0; x <= cSP.xMax; x++)
                    {
                        for(int y = cSP.yMin; y <= cSP.yMax; y++, i++)
                        {
                            if(cTab[i] != whitespaceChar && cTab[i] != borderChar)
                                tetrisMap[x][y] = cTab[i];
                        }
                    }
                    std::cout << "up2end\n";
                    for(int i = 0; i < 8; i++)
                        currentShapeInfo[i] = cTab[i];
                    currentShapePos = cSP;
                }
                break;
            case 4:
                {
                    Vector2Range cSP = currentShapePos;
                    int x5Pos = cSP.xMax, y5Pos = cSP.yMin + 1;

                    if (x5Pos < 1 || x5Pos + 1 >= MAX_HEIGHT || y5Pos < 1 || y5Pos + 2 >= MAX_WIDTH)
                    {
                        std::cout << "up4border\n";
                        cannotMove = true;
                        break;
                    }
                    for(int y = y5Pos, i = cSP.xMax; y < y5Pos + 2; y++, i--)
                    {
                        for(int x = x5Pos + 1, j = cSP.yMax; x >= x5Pos; x--, j--)
                        {
                            if(tetrisMap[x+1][y] != ' ' && tetrisMap[i][j] != ' ' && x + 1 > cSP.xMax)
                            {
                                cannotMove = true;
                                break;
                            }
                        }
                        if(cannotMove)
                            break;
                    }
                    if(cannotMove)
                        break;

                    char cTab[8] =
                    {
                        currentShapeInfo[4],    // 1|0|4
                        currentShapeInfo[0],    // 3|0|0
                        currentShapeInfo[5],    // 2|1|5
                        currentShapeInfo[1],    // 2|0|1
                        currentShapeInfo[6],    // 1|1|6
                        currentShapeInfo[2],    // 1|0|2
                        currentShapeInfo[7],    // 0|1|7
                        currentShapeInfo[3]     // 3|1|3
                    };
                    for(int x = cSP.xMin; x <= cSP.xMax; x++)
                    {
                        for(int y = cSP.yMin; y <= cSP.yMax; y++)
                        {
                            if(x != 0 && x != MAX_HEIGHT - 1 && y != 0 && y != MAX_WIDTH - 1)
                                tetrisMap[x][y] = whitespaceChar ? whitespaceChar : tetrisMap[x][y];
                        }
                    }

                    cSP.xMin = x5Pos - 2; cSP.xMax = x5Pos + 1;
                    cSP.yMin = y5Pos; cSP.yMax = y5Pos + 1;
                    for(int x = cSP.xMin, i = 0; x <= cSP.xMax; x++)
                    {
                        for(int y = cSP.yMin; y <= cSP.yMax; y++, i++)
                        {
                            if(cTab[i] != whitespaceChar && cTab[i] != borderChar)
                                tetrisMap[x][y] = cTab[i];
                        }
                    }
                    for(int i = 0; i < 8; i++)
                    {
                        currentShapeInfo[i] = cTab[i];
                    }
                    currentShapePos = cSP;
                }
                break;
            default:
                debug.Log("Invalid shape of element!");
                debug.PrintLastLog();
                break;
            }
            break;
        case KEY_DOWN:
            {
                Vector2Range cSP = currentShapePos;
                if (cSP.xMax < 0 || cSP.yMin < 0 || cSP.yMax >= MAX_WIDTH)
                {
                    std::cout << "downborder\n";
                    waitForNewElement = false;
                    cannotMove = true;
                    break;
                }
                for(int y = cSP.yMin; y <= cSP.yMax; y++)
                {
                    for(int x = cSP.xMax; x >= cSP.xMax; x--)
                    {
                        if(tetrisMap[x+1][y] != ' ' && tetrisMap[x][y] != ' '
                           && !(tetrisMap[x+1][y] == borderChar && tetrisMap[x][y] == borderChar))
                        {
                            std::cout << x << y << tetrisMap[x][y] << tetrisMap[x+1][y];
                            std::cout << "down\n";
                            cannotMove = true;
                            break;
                        }
                    }
                    if(cannotMove)
                        break;
                }
                if(cannotMove)
                {
                    waitForNewElement = false;
                    break;
                }
                for(int y = cSP.yMin; y <= cSP.yMax; y++)
                {
                    for(int x = cSP.xMax; x >= cSP.xMin; x--)
                    {
                        if(tetrisMap[x][y] != whitespaceChar && tetrisMap[x][y] != borderChar)
                            tetrisMap[x+1][y] = tetrisMap[x][y];
                        if(tetrisMap[x][y] != borderChar)
                           tetrisMap[x][y] = ' ';
                    }
                }
                cSP.xMin++; cSP.xMax++;
                currentShapePos = cSP;
            }
            break;
        default:
            // maybe debug in future
            break;
        }


        debug.Log("Moving lines.");
        // debug.PrintLastLog();
        if(duration + 1 < clock()/(double) CLOCKS_PER_SEC)
        {
            duration = clock()/(double) CLOCKS_PER_SEC;
            Vector2Range cSP = currentShapePos;
            if (cSP.xMax < 1 || cSP.yMin < 1 || cSP.yMax >= MAX_WIDTH)
            {
                std::cout << "moveborder\n";
                waitForNewElement = false;
                cannotMove = true;
            }
            if(cannotMove == false)
                for(int y = cSP.yMin; y <= cSP.yMax; y++)
                {
                    for(int x = cSP.xMax; x >= cSP.xMax; x--)
                    {
                        if(tetrisMap[x+1][y] != ' ' && tetrisMap[x][y] != ' '
                           && !(tetrisMap[x+1][y] == borderChar && tetrisMap[x][y] == borderChar))
                        {
                            std::cout << "move\n";
                            cannotMove = true;
                            break;
                        }
                    }
                    if(cannotMove)
                        break;
                }
            if(cannotMove)
                waitForNewElement = false;
            else
            {
                for(int y = cSP.yMin; y <= cSP.yMax; y++)
                {
                    for(int x = cSP.xMax; x >= cSP.xMin; x--)
                    {
                        if(tetrisMap[x][y] != whitespaceChar && tetrisMap[x][y] != borderChar)
                            tetrisMap[x+1][y] = tetrisMap[x][y];
                        if(tetrisMap[x][y] != borderChar)
                           tetrisMap[x][y] = ' ';
                    }
                }
                cSP.xMin++; cSP.xMax++;
                currentShapePos = cSP;
            }
        }

        debug.Log("Checking lines.");
        // debug.PrintLastLog();
        if(!waitForNewElement)
        {
            int linesToRemove = 0;
            int linesTR[MAX_HEIGHT - 2] = {0};
            for(int x = MAX_HEIGHT - 2; x > 0; x--)
            {
                int charsToRemove = 0;
                for(int y = 1; y < MAX_WIDTH-1; y++)
                {
                    if(tetrisMap[x][y] != whitespaceChar && tetrisMap[x][y] != borderChar)
                    {
                        charsToRemove++;
                    }
                }
                if(charsToRemove == MAX_WIDTH - 2)
                {
                    if(tetrisMap[x][1] == removeLineChar)
                    {
                        for(int y = 1; y < MAX_WIDTH-1; y++)
                        {
                            tetrisMap[x][y] = whitespaceChar;
                        }
                        waitForNewElement = false;
                    }
                    else
                    {
                        for(int y = 1; y < MAX_WIDTH-1; y++)
                        {
                            tetrisMap[x][y] = removeLineChar;
                        }
                         linesToRemove++;
                         linesTR[x] = 1;
                         waitForNewElement = true;
                    }
                    delayMS -= 500;
                }
            }
            if(waitForNewElement)
            {
                for(int i = 0, j = 0; i < linesToRemove; i++, j++)
                {
                    if(linesTR[j] == 0)
                    {
                        i--;
                        continue;
                    }
                    for(int x = j; x > 0; x--) // just check if bottom is free or removed high
                    {
                        for(int y = 1; y < MAX_WIDTH; y++)
                        {
                            tetrisMap[x][y] = tetrisMap[x-1][y];
                            tetrisMap[x-1][y] = whitespaceChar;
                        }
                    }
                }
            }
            if(linesToRemove > 0)
            {
                debug.Log("Writing removed lines.");
                // debug.PrintLastLog();
                points += 100 * pow(2, linesToRemove - 1);
            }
        }
        debug.Log("Writing elements.");
        // debug.PrintLastLog();
        for(int x = 0; x < MAX_HEIGHT; x++) // just check if bottom is free or removed high
        {
            std::cout << "\t\t";
            for(int y = 0; y < MAX_WIDTH; y++)
            {
                std::cout << tetrisMap[x][y];
            }
            std::cout << "\n";
        }

        std::cout << "\n\n\t\tPOINTS: " << points << "\n";
        std::cout << "\t\tTIMER: " << (std::clock() - startTimer) / (double)CLOCKS_PER_SEC << "\n";
        debug.Log("Elements written correctly.");
        // debug.PrintLastLog();
        //std::this_thread::sleep_for(std::chrono::milliseconds(delayMS));
    }
    debug.PrintLogs();

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    system("pause");
    return 0;
}
