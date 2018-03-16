/*
 * CS 106B/X Boggle
 * This file declares the graphical user interface for Boggle.
 * The static functions appear in a class, so to use them, you must write e.g.
 *
 * BoggleGUI::initialize(4, 4);
 * BoggleGUI::setScore(42, BoggleGUI::HUMAN);
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2016/10/25
 * - 16au initial version; heavily refactored into fully graphical UI
 *
 * @author original version by Eric Roberts and Julie Zelenski;
 *         modified by Marty Stepp for Autumn 2013 and Winter 2014;
 *         further modified by Marty Stepp for Autumn 2016 106X
 */

#ifndef _bogglegui_h
#define _bogglegui_h

#include <string>
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "grid.h"
#include "gtypes.h"
#include "gwindow.h"
#include "lexicon.h"
#include "vector.h"

class BoggleGUI {
private:
    /* This enumeration distinguishes the human and computer players. */
    enum Player { HUMAN, COMPUTER };

public:
    /* Constant for minimum allowed word length. */
    static const int MIN_WORD_LENGTH = 4;

    /* Largest allowed board size. */
    static const int BOARD_SIZE_MAX = 4;

    /* Smallest allowed board size. */
    static const int BOARD_SIZE_MIN = 4;

    /* Default board size of 4 to indicate 4x4 board. */
    static const int BOARD_SIZE_DEFAULT = BOARD_SIZE_MIN;

    /*
     * Restores any highlighted cubes to their normal color.
     */
    static void clearHighlighting();

    /*
     * Called by main() to listen to events.
     * Not to be called by students.
     */
    static void eventLoop();

    /*
     * Initializes the Boggle graphics package and its graphics window,
     * using the given number of rows and columns on the board.
     * An error is raised if either dimension is < 0 or > MAX_DIMENSION.
     */
    static void initialize(int rows = BOARD_SIZE_DEFAULT, int cols = BOARD_SIZE_DEFAULT);

    /*
     * Plays a sound effect located in the given file.
     * (NOTE: Sound playing is broken in the current version of the Stanford C++
     *  back-end library, so you probably should not use this function.  Sorry.)
     */
    static void playSound(const std::string& filename);

    /*
     * Adds the given number of points to the current total in the score label
     * for the given player in the game.
     */
    static void scorePoints(int points, Player player);
    static void scorePointsComputer(int points);
    static void scorePointsHuman(int points);

    /*
     * Sets the Boggle game to pause for the given number of milliseconds
     * after toggling the highlight on any letter cube.
     * If set to <= 0, there will be no delay.
     * The default is 100ms, as defined in DEFAULT_ANIMATION_DELAY below.
     */
    static void setAnimationDelay(int ms);

    /*
     * Sets whether the letter cube at the given row/column should be displayed
     * with a bright highlight color.  This is used when the human player is
     * searching for words on the game board.
     */
    static void setHighlighted(int row, int col, bool highlighted = true, bool animate = true);

private:
    /* States that the game/GUI can be in. */
    enum State {
        STATE_ASK_BOARD_SIZE,    // game is waiting for user to type board size
        STATE_ASK_RANDOM_BOARD,  // game is asking Y/N if user wants random board
        STATE_TYPE_BOARD,        // game is waiting for user to type board string
        STATE_HUMAN_TURN,        // game is waiting for human player to type words
        STATE_COMPUTER_TURN,     // game is letting computer player find words
        STATE_GAME_OVER          // game is over; asking Y/N to play again
    };

    /*
     * Removes any prompt for input from GUI.
     */
    static void askNone();

    /*
     * Prompts user to type text into text field.
     */
    static void askText(const std::string& prompt, const std::string& initialValue = "");

    /*
     * Prompts user to choose Yes or No by clicking Y/N buttons.
     */
    static void askYesOrNo(const std::string& prompt);

    /*
     * Internal helper to compute sizes of various GUI shapes.
     */
    static void calculateGeometry();

    /*
     * Closes and discards the graphical window.
     * This differs from shutdown() in that you can call close() and then call
     * init() again to make a new window (perhaps of a different size)
     * while shutdown() totally closes down the Stanford C++ graphics subsystem.
     */
    static void close();

    /*
     * Number of columns in the game board.
     */
    static int columnCount();

    /*
     * Number of total cubes in the game board.
     * Equal to rowCount * columnCount.
     */
    static int cubeCount();

    /*
     * Internal helper to make sure GUI is initialized.
     * Throws an ErrorException if the GUI has not been initialized.
     */
    static void ensureInitialized();

    /*
     * Helpers to handle events while game is in various states.
     */
    static void handleEvent_askRandomBoard(GActionEvent& event);
    static void handleEvent_askBoardSize(GActionEvent& event);
    static void handleEvent_typeBoard(GActionEvent& event);
    static void handleEvent_humanTurn(GActionEvent& event);
    static void handleEvent_gameOver(GActionEvent& event);

    /*
     * Returns true if the Boggle GUI has already been initialized.
     */
    static bool isInitialized();

    /*
     * Draws the specified letter on the face of the cube at position (row, col).
     * The cubes are numbered from top to bottom left to right starting with zero.
     * The upper left corner is is (0, 0); the lower right is (rowCount-1, columnCount-1).
     *
     * An error is raised if row or col is out of range for the board.
     */
    static void labelCube(int row, int col, char letter);

    /*
     * Draws the specified letters on the face of all cubes of the board.
     * You can pass a 16-letter string or a string with line breaks after each four.
     * An error is raised if the string is not the right length.
     */
    static void labelAllCubes(const Grid<char>& letters);
    static void labelAllCubes(const std::string& letters);

    /*
     * Reads boardText into the given grid; returns true if successful.
     */
    static bool makeBoard(Grid<char>& board, const std::string& boardText);

    /*
     * Records the specified word by adding it to the screen display for
     * the specified player.
     * If the given word has already been recorded, adds it again (so don't do that).
     */
    static void recordWord(const std::string& word, Player player);

    /*
     * Clears the GUI's state, including all letters, highlighting, and scores.
     * After a call to reset(), the cubes will be blank, scores will show as 0,
     * and no words will be recorded.
     */
    static void reset();

    /*
     * Number of rows in the game board.
     */
    static int rowCount();

    /*
     * Sets the score label for the given player to display the given
     * number of points.
     */
    static void setScore(int score, Player player);

    /*
     * Sets the Boggle GUI window to display the given text as a status message.
     */
    static void setStatusMessage(const std::string& status, bool isError = false);

    /*
     * Internal helper to draw letter cubes as rounded rectangles.
     * Creates all cubes in position, including a blank letter centered in
     * the middle of each cube.  Initially the cubes are not highlighted.
     */
    static void setupLetterCubes();

    /*
     * Internal helper to draw player labels.
     * Labels player word list with specified name and draws a line underneath.
     * Also sets up the score label for the specified player.
     */
    static void setupPlayerLabels(Player player, const std::string& name);

    /*
     * Closes the GUI window and frees all associated memory.
     */
    static void shutdown();

    /*
     * Constants
     * ---------
     * To configure fixed spacing, fonts, etc.
     */
    static const bool CONSOLE_OUTPUT = true;                // true to print debug messages
    static const int BOGGLE_WINDOW_HEIGHT = 500;            // window size in px
    static const int BOGGLE_WINDOW_WIDTH  = 800;
    static const int DEFAULT_ANIMATION_DELAY = 100;         // delay in ms
    static const int SCORE_FONT_SIZE = 18;
    static const int WORD_FONT_SIZE = 11;

    static constexpr const double BOARD_BORDER = 8;         // width of border around the cubes
    static constexpr const double FONT_PERCENTAGE = 0.6;    // percentage of cube used for letter
    static constexpr const double HUMAN_PERCENTAGE = 0.38;  // percentage of space for word lists to apportion to human
    static constexpr const double INDENT = 20;              // indent at vertical edges of window
    static constexpr const double LABEL_DY = -5;            // adjustment for label text
    static constexpr const double LABEL_HEIGHT = 40;        // space at top of window for labels
    static constexpr const double WORD_DY = 2;              // adjustment for word list

    static const std::string BOGGLE_WINDOW_TITLE;
    static const std::string CUBE_FONT;           // font for letters on cube face
    static const std::string DICTIONARY_FILE;
    static const std::string SCORE_FONT;          // font for scoreboard names & scores
    static const std::string WORD_FONT;

    // colors used on the GUI, as hex strings
    static const int BOARD_COLOR       = 0x326BF6;
    static const int DIE_COLOR         = 0xE6E6E6;
    static const int LETTER_COLOR      = 0x023B46;
    static const int DIE_COLOR_H       = 0xFFEE66;   // _H = highlighted colors
    static const int LETTER_COLOR_H    = 0x023BD6;
    static const int WORD_COLOR        = 0x02805D;
    static const int LABEL_COLOR       = 0x028040;
    static const int LABEL_COLOR_ERROR = 0xA22020;

    // letters on all 6 sides of every cube in 4x4 standard Boggle
    static const Vector<std::string> LETTER_CUBES;

    // letters on every cube in 5x5 "Big Boggle" version
    static const Vector<std::string> LETTER_CUBES_BIG;

    // letters on every cube in 6x6 "Super Big Boggle" version
    static const Vector<std::string> LETTER_CUBES_SUPER_BIG;

    // (static) member variables

    /*
     * A static struct is used to gather the state that is local to this module.
     * Mostly this is geometry calculated to fit the cubes, word lists,
     * and labels neatly within the current size of the graphics window.
     * In addition, the module tracks the count of words listed for each player
     * (to place additional words in the correct location in the displayed word
     * list) and the scores for each player (which must be saved and erased
     * before updating). There is also a 2-d array of the letters currently
     * showing on the cubes, to enable drawing them inverted for the
     * highlighting function.
     *
     * We'd like to use GRectangle instead, but it doesn't support setter methods.
     */
    struct RectStruct { double x, y, w, h; };

    /*
     * Maintains information about a single letter cube on the screen.
     * Used so that the letter cube's color and letter can be updated without
     * needing to redraw them over the existing one.
     */
    struct CubeInfo {
        GRoundRect* rect;
        GLabel* label;
        CubeInfo() {
            rect = nullptr;
            label = nullptr;
        }
    };

    // Global pointers to graphical objects such as the graphics window,
    // and collections of objects for letter cubes and words recorded.
    // These are used to update/clear the letters and words as the game goes on.
    static GWindow* window;
    static GLabel* promptLabel;
    static GTextField* gtextfield;
    static GButton* yesButton;
    static GButton* noButton;
    static GLabel* humanScoreLabel;
    static GLabel* computerScoreLabel;
    static GLabel* statusLabel;
    static Grid<CubeInfo> letterCubes;
    static Vector<GLabel*> recordedWordLabels;
    static int animationDelay;

    static int totalScore[2];        // points scored by each player
    static int wordCount[2];         // words displayed for each player
    static RectStruct scoreBox[2];   // rectangle enscribing label + score for each player
    static RectStruct statusBox;     // rectangle enscribing current game status
    static double wordColumnWidth;   // width of word list columns
    static double cubeSize;          // size of cube
    static double fontSize;          // font size used for labeling cubes
    static RectStruct boardRect;     // rectangle enscribed the cubes
    static State gameState;
    static Grid<char> gameBoard;
    static Lexicon dictionary;
    static Set<std::string> humanWords;
    static int lastBoardSize;
    static std::string lastBoard;
};

#endif // _bogglegui_h
