//
// Created by Aman LaChapelle on 10/28/16.
//
// multiplotter
// Copyright (c) 2016 Aman LaChapelle
// Full license at multiplotter/LICENSE.txt
//

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MULTIPLOTTER_PLOTTER_HPP
#define MULTIPLOTTER_PLOTTER_HPP

#include <Python.h>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <numeric>

#ifndef python
  #define python "/usr/local/bin/python"
#endif


/**
 * @file include/Plotter.hpp
 * @brief Header file for the plotter class
 */


/**
 * @class Plotter
 * @brief Holds the methods associated with plotting points.
 *
 * This class holds all the methods associated with calling the matplotlib library and using it to plot 
 * data generated or accessed by C++ code.
 */
class Plotter {
  std::string cwd;
  std::string which_python;

  PyObject *figure;
  PyObject *plot;
  PyObject *savefig;
  PyObject *draw;
  PyObject *show;
  PyObject *close;

  std::vector<std::string> figures;

public:
  /**
   * Constructor - Takes as an argument the current directory as well as which python interpreter to use.
   *
   * @param cwd Current Working Directory
   * @param which_python Path to the python interpreter we want to use
   */
  Plotter(std::string cwd = ".", std::string which_python = python);

  /**
   * Destructor - Decrefs python variables (hopefully to zero) and finalizes the interpreter.
   */
  ~Plotter();

private:
  /**
   * Internal method - checks if a PyObject is callable, throws an error if it isn't.
   *
   * @param obj PyObject to check
   */
  void check_callable(PyObject *obj);

  std::vector<double> linspace(double start, double end, long num);

public:

  /**
   * Plots the data!  Takes a map (dictionary) where the key is the format and a number to distinguish the entries
   * and the value is a tuple of vectors that represent x and y.  Throws assertion error if x and y are not the same
   * length.
   *
   * @param data The map of data values to plot
   * @param name The name to assign the plot if it's being saved.
   * @param s Whether to show the plot or not.  If true, then the plot is not saved to a file.  If false, the
   *          plot is saved to a file.
   */
  void Plot(std::map<std::string, std::tuple<std::vector<double>, std::vector<double>>> data, std::string name);

  /**
   * Plots an input vector against a linspace in x.  Simplest version of the plot command, can be used to plot simple
   * arrays for examples.
   *
   * @param ydata Input Y array
   * @param fmt Format - check matplotlib.pyplot for acceptable formats
   * @param name Name of the plot - required to keep track of everything
   */
  void Plot(std::vector<double> ydata, std::string name, std::string fmt = "o");

  /**
   * Shows all the figures created in one session of the plotter
   */
  void Show();

  /**
   * Allows selection of a specific figure to be saved to a file according to its name, or alternatively to save
   * all the figures created in one session of the plotter to files.
   *
   * @param savedir Where to save the plot
   * @param which Which plot to save
   */
  void Save(std::string savedir = "plots/", std::string which = "", std::string format = ".png");


};



#endif //MULTIPLOTTER_PLOTTER_HPP
