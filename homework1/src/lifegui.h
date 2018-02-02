/*
 * CS 106B/X, Game of Life
 * This instructor-provided file declares a GUI display needed for
 * the Game of Life assignment.
 * See lifegui.cpp for the implementation of each member.
 *
 * Based on an earlier version written by Julie Zelenski,
 * with later modifications by Cynthia Lee + TAs.
 *
 * NOTE: Your turned-in files should work with an unmodified version
 * of all instructor-provided code files.
 *
 * author: Marty Stepp
 * version: 2016/09/29
 */

#ifndef _lifegui_h
#define _lifegui_h

#include "gwindow.h"

/**
 * The LifeGUI class is a graphical user interface for the Game of Life program.
 */
class LifeGUI {
public:
    /**
     * Erases any filled circles for cells in the grid, reverting it to its
     * initial state with only the black outline and gray grid lines drawn.
     * Clients can call this between generations.
     */
    static void clear();

    /**
     * Draws a black circle for the cell at the specific row and column.
     * Rows and columns are specified using zero-based indexing and (0,0) is
     * the upper-left corner.
     * Note that the cell will not immediately appear on the screen; the
     * client must also call repaint() to see any newly drawn cells.
     * The idea is that on each generation of your simulation, you should call
     * fillCell on all living cell locations, and then call repaint once to
     * see all of the changes.
     * If the location given is not in bounds, an error is thrown.
     * A cell cannot be un-drawn; to make that area white again, call clear().
     */
    static void fillCell(int row, int col);

    /**
     * Sets up the state of the GUI and pops up the GUI window on the screen.
     * This needs to be called only once by the client.
     * If this has not been called but other methods are called that depend on
     * the GUI being initialized (such as resize, fillCell, etc.), initialize
     * will be automatically called by the given other method.
     */
    static void initialize();

    /**
     * Instructs the GUI to redraw itself.
     * You should call this after you are done filling all cells for a given
     * generation.  The reason that the cells are not filled instantly is
     * because that leads to slow graphical performance.
     */
    static void repaint();

    /**
     * This will erase the graphics window completely, draw a black
     * border around the simulation rectangle which is centered in the
     * window, and draw light gray grid lines around each cell.
     * The grid cells will be sized as large as will fit given the
     * grid geometry. Grids with more rows and columns will use smaller
     * cells. This function can be used at the beginning of a simulation or
     * between generations to clear the window before drawing the next generation.
     */
    static void resize(int rows, int cols);

    /**
     * Saves a screenshot of the GUI to the given file.
     */
    static void screenshot(const std::string& filename);

    /**
     * Sets whether the Life GUI is enabled (true) or disabled (false).
     * Generally clients should not need to call this method, but we have
     * included it to facilitate grading scripts and other private features.
     */
    static void setEnabled(bool enabled = true);

    /**
     * Sets the GUI to hide itself behind other windows on screen.
     * Clients generally do not need to call this.
     */
    static void setToBack(bool toBack = true);

    /**
     * Closes the window and shuts down the GUI.
     * Clients generally do not need to call this.
     */
    static void shutdown();

private:
    /*
     * Private constructor prevents client construction.
     * This class is meant to be accessed via static methods.
     */
    LifeGUI();

    /* private internal data; clients should not use/modify these */
    static GWindow* window;
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;
    static int cellDiameter;
    static int rows;
    static int cols;
    static int upperLeftX;
    static int upperLeftY;
    static bool isEnabled;
    static bool isInitialized;
    static bool isToBack;
};

#endif // _lifegui_h
