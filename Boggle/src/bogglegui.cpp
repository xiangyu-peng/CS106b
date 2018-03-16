/*
 * CS 106B/X Boggle
 * This file implements the graphical user interface for Boggle.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2018/02/01
 * - 18wi 106B version; minor refactor for B board sizes
 * @version 2016/10/25
 * - 16au initial version; heavily refactored into fully graphical UI
 *
 * @author original version by Eric Roberts and Julie Zelenski;
 *         modified by Marty Stepp for Autumn 2013 and Winter 2014;
 *         further modified by Marty Stepp for Autumn 2016 106X
 *
 * Known open issue: Leaks memory for all GObjects created.  Oops.
 * I would fix this, but I don't have time to figure out whether
 * freeing them would break things in the Java Back End.  -- Marty
 */

#include "bogglegui.h"
#include "error.h"
#include "filelib.h"
#include "gevents.h"
#include "sound.h"
#include "strlib.h"


// declare/initialize static constants
const std::string BoggleGUI::BOGGLE_WINDOW_TITLE = "CS 106B/X Boggle";
const std::string BoggleGUI::DICTIONARY_FILE = "dictionary.txt";
const std::string BoggleGUI::CUBE_FONT = "Helvetica-Bold";
const std::string BoggleGUI::SCORE_FONT = "Helvetica";
#ifdef _WIN32
const std::string BoggleGUI::WORD_FONT = "Arial";
#else
const std::string BoggleGUI::WORD_FONT = "Geneva";
#endif

const Vector<std::string> BoggleGUI::LETTER_CUBES {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const Vector<std::string> BoggleGUI::LETTER_CUBES_BIG {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

const Vector<std::string> BoggleGUI::LETTER_CUBES_SUPER_BIG {
    "AAAFRS", "AAEEEE", "AAEEOO", "AAFIRS", "ABDEIO", "ADENNN",
    "AEEEEM", "AEEGMU", "AEGMNN", "AEILMN", "AEINOU", "AFIRSY",
    /* "AnErHeInQuTh" */ "AEHIQT", "BBJKXZ", "CCENST", "CDDLNN", "CEIITT", "CEIPST",
    "CFGNUY", "DDHNOT", "DHHLOR", "DHHNOW", "DHLNOR", "EHILRS",
    "EIILST", "EILPST", /* "EIO###" */ "EIOEIO", "EMTTTO", "ENSSSU", "GORRVW",
    "HIRSTV", "HOPRST", "IPRSYY", /* "JKQuWXZ" */ "JKQWXZ", "NOOTUW", "OOOTTU",
};

// declare/initialize static member variables
GWindow* BoggleGUI::window = nullptr;
GLabel* BoggleGUI::promptLabel = nullptr;
GTextField* BoggleGUI::gtextfield = nullptr;
GButton* BoggleGUI::yesButton = nullptr;
GButton* BoggleGUI::noButton = nullptr;
GLabel* BoggleGUI::humanScoreLabel = nullptr;
GLabel* BoggleGUI::computerScoreLabel = nullptr;
GLabel* BoggleGUI::statusLabel = nullptr;
Grid<BoggleGUI::CubeInfo> BoggleGUI::letterCubes;
Vector<GLabel*> BoggleGUI::recordedWordLabels;
int BoggleGUI::animationDelay = BoggleGUI::DEFAULT_ANIMATION_DELAY;
int BoggleGUI::totalScore[2] = {0, 0};
int BoggleGUI::wordCount[2] = {0, 0};
BoggleGUI::RectStruct BoggleGUI::scoreBox[2];
BoggleGUI::RectStruct BoggleGUI::statusBox;
double BoggleGUI::wordColumnWidth;
double BoggleGUI::cubeSize;
double BoggleGUI::fontSize;
BoggleGUI::RectStruct BoggleGUI::boardRect;
BoggleGUI::State BoggleGUI::gameState;
Grid<char> BoggleGUI::gameBoard;
Lexicon BoggleGUI::dictionary;
Set<std::string> BoggleGUI::humanWords;
int BoggleGUI::lastBoardSize = BoggleGUI::BOARD_SIZE_DEFAULT;
std::string BoggleGUI::lastBoard = "";


// these are the functions you must implement in bogglesearch.cpp
// (DO NOT MODIFY THESE HEADERS IN ANY WAY!)
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, std::string word);

Set<std::string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<std::string>& humanWords);


// implementation of static member functions
static Grid<char> mystery(int xyz, const Vector<std::string>& v);

void BoggleGUI::askNone() {
    promptLabel->setText("<html>&nbsp;</html>");   // empty, but retain its height
    window->removeFromRegion(yesButton, GWindow::REGION_NORTH);
    window->removeFromRegion(noButton, GWindow::REGION_NORTH);
    window->removeFromRegion(gtextfield, GWindow::REGION_NORTH);
}

void BoggleGUI::askText(const std::string& prompt, const std::string& initialValue) {
    std::string goodPrompt = trim(prompt) + " ";
    promptLabel->setText(goodPrompt);
    if (CONSOLE_OUTPUT && !trim(prompt).empty()) {
        std::cout << goodPrompt;
        std::cout.flush();
    }
    window->removeFromRegion(yesButton, GWindow::REGION_NORTH);
    window->removeFromRegion(noButton, GWindow::REGION_NORTH);
    window->addToRegion(gtextfield, GWindow::REGION_NORTH);
    yesButton->setAccelerator("");
    noButton->setAccelerator("");
    gtextfield->setText(initialValue);
    gtextfield->setEnabled(true);
    gtextfield->requestFocus();
}

void BoggleGUI::askYesOrNo(const std::string& prompt) {
    std::string goodPrompt = trim(prompt) + " ";
    promptLabel->setText(goodPrompt);
    if (CONSOLE_OUTPUT && !trim(prompt).empty()) {
        std::cout << goodPrompt;
        std::cout.flush();
    }
    yesButton->setEnabled(true);
    noButton->setEnabled(true);
    yesButton->setAccelerator("y");
    noButton->setAccelerator("n");
    window->removeFromRegion(gtextfield, GWindow::REGION_NORTH);
    window->addToRegion(yesButton, GWindow::REGION_NORTH);
    window->addToRegion(noButton, GWindow::REGION_NORTH);
    yesButton->requestFocus();
}

/*
 * This internal helper does all the messy math to work out how to divide
 * up the space within the current graphics window to neatly fit the board,
 * the cubes, and the word lists.
 * This function does not actually add any GObjects to the GWindow;
 * but it does set the state of the ' structure to be used later.
 */
void BoggleGUI::calculateGeometry() {
    double boardSize = std::min(window->getWidth() / 3 - 2 * INDENT,
                                window->getHeight() - 2 * LABEL_HEIGHT);
    cubeSize = std::min((boardSize - BOARD_BORDER) / rowCount(),
                        (boardSize - BOARD_BORDER) / columnCount());
    boardRect.w = boardRect.h = boardSize;
    boardRect.y = LABEL_HEIGHT + LABEL_HEIGHT;
    double leftover = window->getWidth() - boardRect.w - 2 * INDENT;
    scoreBox[HUMAN].x = INDENT;
    scoreBox[HUMAN].y = scoreBox[COMPUTER].y = boardRect.y;
    scoreBox[HUMAN].h = scoreBox[COMPUTER].h = LABEL_HEIGHT;
    scoreBox[HUMAN].w = leftover * HUMAN_PERCENTAGE;
    boardRect.x = scoreBox[HUMAN].x + scoreBox[HUMAN].w + INDENT;
    scoreBox[COMPUTER].x = boardRect.x + boardRect.w + INDENT;
    scoreBox[COMPUTER].w = window->getWidth() - scoreBox[COMPUTER].x - INDENT;
    statusBox.x = INDENT;
    statusBox.y = LABEL_HEIGHT;
    statusBox.w = window->getWidth() - 2 * INDENT;
    statusBox.h = LABEL_HEIGHT;

    // figure out an ideal font size by creating labels of different sizes
    GLabel label("mmmmm");
    label.setFont(WORD_FONT + "-" + integerToString(WORD_FONT_SIZE));
    wordColumnWidth = label.getWidth();

    for (fontSize = 8; fontSize < 48; fontSize++) {
        GLabel label2("M");
        label2.setFont(CUBE_FONT + "-" + integerToString(int(fontSize)));
        if (label2.getWidth() > FONT_PERCENTAGE * cubeSize) {
            break;
        }
    }
}

void BoggleGUI::clearHighlighting() {
    ensureInitialized();
    for (int row = 0; row < rowCount(); row++) {
        for (int col = 0; col < columnCount(); col++) {
            setHighlighted(row, col, false, /* animate */ false);
        }
    }
}

void BoggleGUI::close() {
    if (isInitialized()) {
        window->setVisible(false);
        window->close();
        delete window;
        window = nullptr;
        humanScoreLabel = nullptr;
        computerScoreLabel = nullptr;
        statusLabel = nullptr;
        letterCubes.resize(0, 0);
        recordedWordLabels.clear();
    }
}

int BoggleGUI::columnCount() {
    return gameBoard.numCols();
}

int BoggleGUI::cubeCount() {
    return rowCount() * columnCount();
}

void BoggleGUI::ensureInitialized() {
    if (!isInitialized()) {
        error("BoggleGUI has not been initialized yet");
    }
}

void BoggleGUI::eventLoop() {
    initialize(BOARD_SIZE_DEFAULT, BOARD_SIZE_DEFAULT);

    if (BOARD_SIZE_MIN < BOARD_SIZE_MAX) {
        gameState = STATE_ASK_BOARD_SIZE;
        askText("Board size? ", integerToString(lastBoardSize));
    } else {
        // fixed board size
        gameState = STATE_ASK_RANDOM_BOARD;
        askYesOrNo("Generate a random board?");
    }

    if (dictionary.isEmpty()) {
        dictionary.addWordsFromFile(DICTIONARY_FILE);
    }

    // event loop; split up into helpers for better decomposition
    while (true) {
        GEvent event = waitForEvent(ACTION_EVENT | WINDOW_EVENT);
        if (event.getEventClass() == ACTION_EVENT) {
            GActionEvent actionEvent(event);

            // echo user input for logging and debugging
            if (CONSOLE_OUTPUT) {
                if (actionEvent.getSource() == yesButton) {
                    std::cout << "Y" << std::endl;
                } else if (actionEvent.getSource() == noButton) {
                    std::cout << "N" << std::endl;
                } else if (actionEvent.getSource() == gtextfield) {
                    std::cout << gtextfield->getText() << std::endl;
                }
            }

            // process action event depending on game state
            if (gameState == STATE_ASK_BOARD_SIZE) {
                handleEvent_askBoardSize(actionEvent);
            } else if (gameState == STATE_ASK_RANDOM_BOARD) {
                handleEvent_askRandomBoard(actionEvent);
            } else if (gameState == STATE_TYPE_BOARD) {
                handleEvent_typeBoard(actionEvent);
            } else if (gameState == STATE_HUMAN_TURN) {
                handleEvent_humanTurn(actionEvent);
            } else if (gameState == STATE_COMPUTER_TURN) {
                //
            } else if (gameState == STATE_GAME_OVER) {
                handleEvent_gameOver(actionEvent);
            }
        } else if (event.getEventClass() == WINDOW_EVENT) {
            if (event.getEventType() == WINDOW_CLOSED) {
                break;
            }
        }
    }
}

void BoggleGUI::handleEvent_askBoardSize(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == gtextfield) {
        bool valid = false;
        if (gtextfield->valueIsInteger()) {
            int boardSize = gtextfield->getValueAsInteger();
            if (BOARD_SIZE_MIN <= boardSize && boardSize <= BOARD_SIZE_MAX) {
                gtextfield->setEnabled(false);
                lastBoardSize = boardSize;
                BoggleGUI::initialize(boardSize, boardSize);
                setStatusMessage("");
                gameState = STATE_ASK_RANDOM_BOARD;
                askYesOrNo("Generate a random board?");
                valid = true;
            }
        }

        if (!valid) {
            setStatusMessage("Board size must be an integer between "
                             + integerToString(BOARD_SIZE_MIN) + " and "
                             + integerToString(BOARD_SIZE_MAX), /* isError */ true);
        }
    }
}

void BoggleGUI::handleEvent_askRandomBoard(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == yesButton) {
        const Vector<std::string>& letterCubeVec =
                rowCount() == BOARD_SIZE_MIN ? LETTER_CUBES :
                rowCount() == BOARD_SIZE_MAX ? LETTER_CUBES_SUPER_BIG :
                LETTER_CUBES_BIG;
        Grid<char> newBoard = mystery(rowCount(), letterCubeVec);

        // verify that this is a legal Boggle board
        if (makeBoard(newBoard, newBoard.toString2D("", "", "", ""))) {
            gameBoard = newBoard;
            labelAllCubes(newBoard);
            gameState = STATE_HUMAN_TURN;

            if (CONSOLE_OUTPUT) {
                std::cout << lastBoard << std::endl;
                std::cout << std::endl;
            }

            setStatusMessage("Human player's turn:");
            askText("Type a word (or Enter to stop): ");
        } else {
            setStatusMessage("Invalid board.", /* isError */ true);
        }
    } else if (actionEvent.getSource() == noButton) {
        if (gameState == STATE_ASK_RANDOM_BOARD) {
            gameState = STATE_TYPE_BOARD;
            askText("Type the " + integerToString(cubeCount())
                    + " letters on the board:", lastBoard);
        }
    }
}

void BoggleGUI::handleEvent_typeBoard(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == gtextfield) {
        Grid<char> newBoard(rowCount(), columnCount());
        std::string text = trim(toUpperCase(gtextfield->getText()));
        bool valid = false;
        if (makeBoard(newBoard, text)) {
            gameBoard = newBoard;
            gtextfield->setText("");
            labelAllCubes(text);
            gameState = STATE_HUMAN_TURN;

            if (CONSOLE_OUTPUT) {
                std::cout << std::endl;
            }

            setStatusMessage("Human player's turn:");
            askText("Type a word (or Enter to stop): ");
            valid = true;
        }

        if (!valid) {
            setStatusMessage("Board string must be exactly " + integerToString(cubeCount())
                             + " letters from A-Z.", /* isError */ true);
        }
    }
}

void BoggleGUI::handleEvent_humanTurn(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == gtextfield) {
        std::string text = trim(toUpperCase(gtextfield->getText()));
        gtextfield->setEnabled(false);

        if (humanWords.contains(text)) {
            setStatusMessage("Duplicate word.", /* isError */ true);
            gtextfield->setText("");
            gtextfield->setEnabled(true);
            gtextfield->requestFocus();
        } else if (text.length() > 0) {
            bool valid = true;
            for (int i = 0; i < (int) text.length(); i++) {
                if (text[i] < 'A' || text[i] > 'Z') {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                valid = false;
                setStatusMessage("Searching ...");

                if (CONSOLE_OUTPUT) {
                    std::cout << "humanWordSearch for \"" << text << "\" ... ";
                    std::cout.flush();
                }

                // calls your recursive function
                bool result = humanWordSearch(gameBoard, dictionary, text);

                if (CONSOLE_OUTPUT) {
                    std::cout << "returned " << std::boolalpha << result << std::endl;
                    if (result) {
                        std::cout << "Human score is now " << totalScore[HUMAN] << "." << std::endl;
                    }
                }

                if (result) {
                    valid = true;
                    humanWords.add(text);
                    setStatusMessage("You found a new word! \"" + text + "\"");
                    BoggleGUI::recordWord(text, HUMAN);
                }
            }

            if (!valid) {
                BoggleGUI::setStatusMessage("Invalid word or not found on the board.", /* isError */ true);
            }

            if (CONSOLE_OUTPUT) {
                std::cout << std::endl;
                std::cout << "Type a word (or Enter to stop): ";
                std::cout.flush();
            }

            gtextfield->setText("");
            gtextfield->setEnabled(true);
            gtextfield->requestFocus();
        } else {
            // human player is done
            if (CONSOLE_OUTPUT) {
                std::cout << std::endl;
            }
            clearHighlighting();

            setStatusMessage("Computer player's turn:");
            gameState = STATE_COMPUTER_TURN;
            askNone();

            if (CONSOLE_OUTPUT) {
                std::cout << "computerWordSearch ..." << std::endl;
            }

            // calls your recursive function
            Set<std::string> allWords = computerWordSearch(gameBoard, dictionary, humanWords);
            if (CONSOLE_OUTPUT) {
                std::cout << "returned " << allWords << std::endl;
                std::cout << "Computer score is " << totalScore[COMPUTER] << "." << std::endl;
                std::cout << std::endl;
            }

            // display words found on GUI
            for (const std::string& word : allWords) {
                recordWord(word, COMPUTER);
            }

            int humanScore = totalScore[HUMAN];
            int computerScore = totalScore[COMPUTER];
            if (humanScore > computerScore) {
                // not likely brah
                setStatusMessage("Wow, you beat me. It must be a glitch.", /* isError */ true);
            } else if (computerScore > humanScore) {
                setStatusMessage("I crushed you, puny human!");
            } else {
                setStatusMessage("We tied. Consider yourself lucky.");
            }

            gameState = STATE_GAME_OVER;
            askYesOrNo("Play again?");
        }
    }
}

void BoggleGUI::handleEvent_gameOver(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == yesButton) {
        // start over; clear words, score, labels of words found
        reset();

        if (BOARD_SIZE_MIN < BOARD_SIZE_MAX) {
            gameState = STATE_ASK_BOARD_SIZE;
            askText("Board size? ", integerToString(lastBoardSize));
        } else {
            // fixed board size
            gameState = STATE_ASK_RANDOM_BOARD;
            askYesOrNo("Generate a random board?");
        }
    } else if (actionEvent.getSource() == noButton) {
        // quit out
        shutdown();
    }
}

void BoggleGUI::initialize(int rows, int cols) {
    if (rows < BOARD_SIZE_MIN || rows > BOARD_SIZE_MAX
            || cols < BOARD_SIZE_MIN || cols > BOARD_SIZE_MAX) {
        error("BoggleGUI::initialize: invalid dimensions");
    }

    gameState = STATE_ASK_BOARD_SIZE;

    if (!window) {
        // create entire GUI
        window = new GWindow(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT, /* visible */ false);
        window->setWindowTitle(BOGGLE_WINDOW_TITLE);
        window->setResizable(true);
        window->setExitOnClose(true);

        promptLabel = new GLabel(" ");
        promptLabel->setFont("*-*-20");
        window->setRegionAlignment(GWindow::REGION_NORTH, GWindow::ALIGN_RIGHT);
        window->addToRegion(promptLabel, GWindow::REGION_NORTH);

        // we kind of hack a horizontal padding into the buttons using HTML and nbsp
        yesButton = new GButton("<html>&nbsp;&nbsp;Yes&nbsp;&nbsp;</html>");
        yesButton->setMnemonic('Y');
        // yesButton->setAccelerator("Ctrl-Y");
        yesButton->setFont("*-20");
        noButton = new GButton("<html>&nbsp;&nbsp;&nbsp;No&nbsp;&nbsp;&nbsp;</html>");
        noButton->setMnemonic('N');
        // yesButton->setAccelerator("Ctrl-N");
        noButton->setFont("*-20");

        gtextfield = new GTextField(20);
        gtextfield->addActionListener();
        gtextfield->setFont("Monospaced-Plain-18");
    }

    gameBoard.resize(rows, cols);
    calculateGeometry();

    wordCount[HUMAN] = 0;
    wordCount[COMPUTER] = 0;

    // draw the Boggle board's blue background
    window->setColor(BOARD_COLOR);
    window->fillRect(boardRect.x, boardRect.y, boardRect.w, boardRect.h);

    if (!humanScoreLabel || computerScoreLabel) {
        // draw the labels of player names and score labels
        setupPlayerLabels(HUMAN, "Human");
        setupPlayerLabels(COMPUTER, "Computer");
    }

    if (!statusLabel) {
        // set up status label
        statusLabel = new GLabel("");
        statusLabel->setFont(SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE));
        statusLabel->setColor(LABEL_COLOR);
        window->add(statusLabel, statusBox.x, statusBox.y);
    }

    // draw the NxN grid of Boggle letter cubes
    setupLetterCubes();

    if (!window->isVisible()) {
        window->center();
        window->setVisible(true);
    }

    gtextfield->requestFocus();

    // clear out any in-progress game state
    reset();
}

bool BoggleGUI::isInitialized() {
    return window != nullptr;
}

void BoggleGUI::labelAllCubes(const Grid<char>& letters) {
    ensureInitialized();
    if (letters.size() != cubeCount()) {
        error("BoggleGUI::labelAllCubes: incorrectly sized grid");
    }
    labelAllCubes(letters.toString2D("", "", "", ""));
}

void BoggleGUI::labelAllCubes(const std::string& letters) {
    ensureInitialized();
    if ((int) letters.length() < cubeCount()) {
        error("BoggleGUI::labelAllCubes: too few letters in string: \"" + letters + "\"");
    }
    int row = 0;
    int col = 0;
    bool labeledAll = false;
    for (int i = 0; i < (int) letters.length(); i++) {
        char letter = letters[i];
        if (!isalpha(letter)) {
            continue;
        }
        labelCube(row, col, letter);
        col++;
        if (col == columnCount()) {
            col = 0;
            row++;
            if (row == rowCount()) {
                labeledAll = true;
                break;
            }
        }
    }
    if (labeledAll) {
        lastBoard = letters;
    } else {
        error("BoggleGUI::labelAllCubes: didn't label every cube from letters string: \"" + letters + "\"");
    }
}

void BoggleGUI::labelCube(int row, int col, char letter) {
    ensureInitialized();
    if (!gameBoard.inBounds(row, col)) {
        error("BoggleGUI::labelCube: invalid row or column");
    }
    if (!isalpha(letter) && letter != ' ') {
        error("BoggleGUI::labelCube: invalid non-alphabetic character");
    }

    GLabel* label = letterCubes[row][col].label;
    label->setLabel(std::string(1, letter));
    if (isalpha(letter)) {
        label->setLocation(
                letterCubes[row][col].rect->getX() + cubeSize/2 - 0.45 * label->getWidth(),
                letterCubes[row][col].rect->getY() + cubeSize/2 + 0.36 * fontSize);
    }
}

// this function's implementation is intentionally dumb so you won't copy it
bool BoggleGUI::makeBoard(Grid<char>& board, const std::string& boardText) {
    if (boardText.length() != cubeCount()) {
        return false;
    }
    std::ostringstream out;
    out << "{";
    for (int i = 0, len = (int) boardText.length(); i < len; i++) {
        if (!isalpha(boardText[i])) {
            return false;
        }
        if (i % board.numCols() == 0) {
            out << "{";
        }
        out << (char) toupper(boardText[i]);
        if (i % board.numCols() == board.numCols() - 1) {
            out << "}";
        }
        if (i < len - 1) {
            out << ", ";
        }
    }
    out << "}";

    std::istringstream in(out.str());
    return (bool) (in >> board);
}

void BoggleGUI::playSound(const std::string& filename) {
    Sound s(filename);
    s.play();
}

void BoggleGUI::recordWord(const std::string& word, Player player) {
    ensureInitialized();
    if (player != HUMAN && player != COMPUTER) {
        error("BoggleGUI::recordWord: invalid player argument");
    }
    GLabel* label = new GLabel(toLowerCase(trim(word)));
    label->setFont(WORD_FONT + "-" + integerToString(WORD_FONT_SIZE));
    label->setColor(WORD_COLOR);
    int wordCountInRow = int(scoreBox[player].w / wordColumnWidth);
    int row = wordCount[player] / wordCountInRow;
    int col = wordCount[player] % wordCountInRow;
    wordCount[player]++;
    window->add(label, scoreBox[player].x + col * wordColumnWidth,
                  scoreBox[player].y + (row + 1) * WORD_FONT_SIZE + WORD_DY);
    if (col == wordCountInRow - 1) {
        window->repaint();
    }
    recordedWordLabels.add(label);
}

void BoggleGUI::reset() {
    ensureInitialized();

    // clear out the counts of words recorded so far and scores
    setStatusMessage("Initializing ...");
    wordCount[HUMAN] = 0;
    wordCount[COMPUTER] = 0;
    setScore(0, HUMAN);
    setScore(0, COMPUTER);
    humanWords.clear();

    // clear the word cubes
    for (int row = 0; row < rowCount(); row++) {
        for (int col = 0; col < columnCount(); col++) {
            labelCube(row, col, ' ');
        }
    }

    // clear out the words found by each player
    for (GLabel* label : recordedWordLabels) {
        window->remove(label);
    }
    recordedWordLabels.clear();
    setStatusMessage("");
}

int BoggleGUI::rowCount() {
    return gameBoard.numRows();
}

void BoggleGUI::scorePoints(int points, Player player) {
    ensureInitialized();
    if (points <= 0) {
        error("BoggleGUI::scorePoints: number of points must be a positive integer");
    }
    if (player != HUMAN && player != COMPUTER) {
        error("BoggleGUI::scorePoints: invalid player argument");
    }
    setScore(totalScore[player] + points, player);
}

void BoggleGUI::scorePointsComputer(int points) {
    scorePoints(points, COMPUTER);
}

void BoggleGUI::scorePointsHuman(int points) {
    scorePoints(points, HUMAN);
}

void BoggleGUI::setAnimationDelay(int ms) {
    animationDelay = ms;
}

void BoggleGUI::setHighlighted(int row, int col, bool highlighted, bool animate) {
    ensureInitialized();
    if (!gameBoard.inBounds(row, col)) {
        error("BoggleGUI::setHighlighted: invalid row or column");
    }
    letterCubes[row][col].rect->setFillColor(highlighted ? DIE_COLOR_H : DIE_COLOR);
    letterCubes[row][col].label->setColor(highlighted ? LETTER_COLOR_H : LETTER_COLOR);
    if (animate && highlighted && animationDelay > 0) {
        pause(animationDelay);
    }
}

void BoggleGUI::setScore(int points, Player player) {
    ensureInitialized();
    totalScore[player] = points;
    GLabel* scoreLabel = player == COMPUTER ? computerScoreLabel : humanScoreLabel;
    scoreLabel->setText(integerToString(points));
}

void BoggleGUI::setStatusMessage(const std::string& status, bool isError) {
    ensureInitialized();
    statusLabel->setLabel(status);
    if (CONSOLE_OUTPUT && !status.empty()) {
        std::cout << status << std::endl;
    }
    statusLabel->setColor(isError ? LABEL_COLOR_ERROR : LABEL_COLOR);
}

void BoggleGUI::setupLetterCubes() {
    double lineWidth = 2;
    double cubeSize = BoggleGUI::cubeSize - lineWidth;
    double cubeY = boardRect.y + BOARD_BORDER / 2;

    // if (cubeCount() != letterCubes.size()) {
        // clear out any old rounded rectangles from the screen
        for (int row = 0; row < letterCubes.numRows(); row++) {
            for (int col = 0; col < letterCubes.numCols(); col++) {
                if (letterCubes[row][col].rect) {
                    window->remove(letterCubes[row][col].rect);
                    window->remove(letterCubes[row][col].label);
                    delete letterCubes[row][col].rect;
                    delete letterCubes[row][col].label;
                    letterCubes[row][col].rect = nullptr;
                    letterCubes[row][col].label = nullptr;
                }
            }
        }
    // }

    letterCubes.resize(rowCount(), columnCount());
    for (int row = 0; row < letterCubes.numRows(); row++) {
        double cubeX = boardRect.x + BOARD_BORDER / 2;
        for (int col = 0; col < letterCubes.numCols(); col++) {
            // display the letter cubes as rounded rectangles
            double cubeRoundRadius = cubeSize / 6;
            GRoundRect* rect = new GRoundRect(cubeX, cubeY, cubeSize,
                                              cubeSize, cubeRoundRadius * 2);
            rect->setLineWidth(lineWidth);
            rect->setColor("Black");
            rect->setFilled(true);
            rect->setFillColor(DIE_COLOR);
            window->add(rect);
            letterCubes[row][col].rect = rect;

            // draw the letter on the cube
            GLabel* label = new GLabel("M");   // start as "M" for getWidth
            label->setFont(CUBE_FONT + "-" + integerToString(int(fontSize)));
            label->setColor(LETTER_COLOR);
            label->setLocation(cubeX + cubeSize / 2 - label->getWidth() / 2,
                               cubeY + cubeSize / 2 + 0.4 * fontSize);
            label->setLabel(" ");
            window->add(label);
            letterCubes[row][col].label = label;
            cubeX += cubeSize + lineWidth;
        }
        cubeY += cubeSize + lineWidth;
    }
}

void BoggleGUI::setupPlayerLabels(Player player, const std::string& name) {
    window->setColor(LABEL_COLOR);
    window->drawLine(scoreBox[player].x, scoreBox[player].y,
                  scoreBox[player].x + scoreBox[player].w,
                  scoreBox[player].y);
    GLabel* label = new GLabel(name);
    label->setFont(SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE));
    window->add(label, scoreBox[player].x, scoreBox[player].y + LABEL_DY);

    GLabel*& scoreLabel = player == COMPUTER ? computerScoreLabel : humanScoreLabel;
    if (scoreLabel) {
        scoreLabel->setText("0");
    } else {
        scoreLabel = new GLabel("0");
    }
    scoreLabel->setFont(SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE));
    scoreLabel->setColor(LABEL_COLOR);
    int offset = 32;
    window->add(scoreLabel,
                scoreBox[player].x + scoreBox[player].w - offset,
                scoreBox[player].y + LABEL_DY);
}

void BoggleGUI::shutdown() {
    if (isInitialized()) {
        if (CONSOLE_OUTPUT) {
            std::cout << "Exiting." << std::endl;
            std::cout.flush();
        }
        close();
        exitGraphics();
    }
}

// what it do?
static Grid<char> mystery(int xyz, const Vector<std::string>& v) {
    Grid<char> g(xyz,xyz);unsigned short boom=xyz*xyz,m=48,s=xyz;
    top:boom--;g[boom/s][boom%s]=v[boom][rand()%(m>>3)];if(boom)goto top;
    shuffle(g);return g;
}

// main function to run the actual program
int main() {
    BoggleGUI::eventLoop();
    return 0;
}
