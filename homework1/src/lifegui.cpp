/*
 * CS 106B/X, Game of Life
 * This instructor-provided file defines a GUI display needed for
 * the Game of Life assignment.
 * See lifegui.h for the declarations of each member.
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

#include "lifegui.h"
#include <cmath>
#include "error.h"
#include "gwindow.h"
#include "map.h"

GWindow* LifeGUI::window = nullptr;
const int LifeGUI::WINDOW_WIDTH = 600;
const int LifeGUI::WINDOW_HEIGHT = 500;
int LifeGUI::cellDiameter = 0;
int LifeGUI::rows = 0;
int LifeGUI::cols = 0;
int LifeGUI::upperLeftX = 0;
int LifeGUI::upperLeftY = 0;
bool LifeGUI::isEnabled = true;
bool LifeGUI::isInitialized = false;
bool LifeGUI::isToBack = false;

LifeGUI::LifeGUI() {
    // empty
}

void LifeGUI::clear() {
    if (!isEnabled) {
        return;
    }
    initialize();

    // clear window
    window->setColor("white");
    window->fillRect(0, 0,
                     window->getWidth() + 1,
                     window->getHeight() + 1);
    window->setColor("black");

    // draw black rect around board area
    window->drawRect(upperLeftX, upperLeftY,
                     cols * cellDiameter,
                     rows * cellDiameter);

    // draw grid lines
    window->setColor("gray");
    for (int r = 1; r < rows; r++) {   // horizontal
        window->drawLine(upperLeftX + 1,
                         upperLeftY + r * cellDiameter,
                         upperLeftX + cols * cellDiameter - 1,
                         upperLeftY + r * cellDiameter);
    }
    for (int c = 1; c < cols; c++) {   // vertical
        window->drawLine(upperLeftX + c * cellDiameter,
                         upperLeftY + 1,
                         upperLeftX + c * cellDiameter,
                         upperLeftY + rows * cellDiameter - 1);
    }
    window->setColor("black");
}

/*
 *
 */
void LifeGUI::fillCell(int row, int col) {
    if (!isEnabled) {
        return;
    }
    initialize();
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        error("LifeGUI::fillCell illegal row/col");
    }
    window->fillOval(upperLeftX + col * cellDiameter + 1,
                     upperLeftY + row * cellDiameter + 1,
                     cellDiameter - 2,
                     cellDiameter - 2);
}

void LifeGUI::initialize() {
    // ensure that this code runs exactly once
    if (!isEnabled) {
        return;
    }
    if (isInitialized) {
        return;
    }
    isInitialized = true;

    window = new GWindow(WINDOW_WIDTH, WINDOW_HEIGHT, /* visible */ false);
    window->setTitle("CS 106X Game of Life");
    window->setRepaintImmediately(false);

    // window->center();
    window->setVisible(true);
    window->setCanvasSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (isToBack) {
        window->toBack();
    }
}

void LifeGUI::repaint() {
    if (!isEnabled) {
        return;
    }
    initialize();
    window->repaint();
}

void LifeGUI::resize(int rows, int cols) {
    if (!isEnabled) {
        return;
    }
    initialize();
    LifeGUI::rows = rows;
    LifeGUI::cols = cols;

    // calculate size of each cell
    double canvasWidth = window->getCanvasSize().getWidth();
    double canvasHeight = window->getCanvasSize().getHeight();
    int hPixelsPerCell = canvasHeight / rows;
    int wPixelsPerCell = canvasWidth / cols;
    cellDiameter = std::min(wPixelsPerCell, hPixelsPerCell);
    upperLeftX = std::max(1.0, (canvasWidth - cols * cellDiameter) / 2);
    upperLeftY = std::max(1.0, (canvasHeight - rows * cellDiameter) / 2);
    LifeGUI::clear();
}

void LifeGUI::screenshot(const std::string& filename) {
    initialize();
    window->saveCanvasPixels(filename);
}


void LifeGUI::setEnabled(bool enabled) {
    isEnabled = enabled;
}

void LifeGUI::setToBack(bool toBack) {
    isToBack = toBack;
}

void LifeGUI::shutdown() {
    if (window) {
        window->setExitOnClose(false);
        window->setVisible(false);
        window->close();
        isInitialized = false;
        delete window;
        window = nullptr;
    }
}
