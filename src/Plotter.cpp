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


#include "../include/Plotter.hpp"

/*
 * CONSTRUCTOR/DESTRUCTOR
 */

Plotter::Plotter(std::string cwd, std::string which_python):
                  which_python(which_python), cwd(cwd) {

  setenv("PYTHONPATH", cwd.c_str(), 1);

  Py_SetProgramName((char *)which_python.c_str());

  Py_Initialize();

  PyObject *plt = PyImport_ImportModule("matplotlib.pyplot");

  if (PyErr_Occurred()){
    PyErr_Print();
  }

  figure = PyObject_GetAttrString(plt, "figure");
  plot = PyObject_GetAttrString(plt, "plot");
  savefig = PyObject_GetAttrString(plt, "savefig");
  draw = PyObject_GetAttrString(plt, "draw");
  show = PyObject_GetAttrString(plt, "show");
  close = PyObject_GetAttrString(plt, "close");

  try{
    check_callable(figure);
    check_callable(plot);
    check_callable(savefig);
    check_callable(show);
    check_callable(draw);
    check_callable(close);
  }
  catch(std::exception &e){
    std::cout << e.what() << std::endl;
  }

}

Plotter::~Plotter() {

  std::for_each(figures.begin(), figures.end(), [this](std::string fig){
    PyObject *args = PyTuple_Pack(1, PyString_FromString(fig.c_str()));
    PyObject_Call(figure, args, NULL);
    args = PyTuple_Pack(0); PyObject_Call(close, args, NULL);
  });

  Py_XDECREF(figure);
  Py_XDECREF(plot);
  Py_XDECREF(savefig);
  Py_XDECREF(show);

  Py_Finalize();
}

/*
 * INTERNAL METHODS
 */

void Plotter::check_callable(PyObject *obj) {
  if (PyCallable_Check(obj)){
    ;
  }
  else{
    if (PyErr_Occurred()){
      PyErr_Print();
    }
    Py_Finalize();
    std::string error = "Object Not Callable! PyObject: ";
    std::string error2 = PyString_AsString(PyObject_Repr(obj));
    error += error2;
    throw std::runtime_error(error);
  }
}

std::vector<double> Plotter::linspace(double start, double end, long num) {
  double interval = (end - start) / (double)(num-1);

  std::vector<double> out (num-1);
  for (int i = 0; i < num-1; i++){
    out[i] = start + i*interval;
  }
  out.push_back(end);

  return out;

}

/*
 * EXTERNAL INTERFACES
 */

void Plotter::Plot(std::map<std::string, std::tuple<std::vector<double>, std::vector<double>>> data,
                   std::string name) {

  PyObject *args;

  for (auto iter = data.begin(); iter != data.end(); iter++){
    PyObject *x;
    PyObject *y;

    std::vector<double> xdata = std::get<0>(iter->second);
    std::vector<double> ydata = std::get<1>(iter->second);

    assert(xdata.size() == ydata.size());

    x = PyList_New(xdata.size());
    for (int i = 0; i < xdata.size(); i++){
      PyList_SetItem(x, i, PyFloat_FromDouble(xdata[i]));
    }

    y = PyList_New(ydata.size());
    for (int i = 0; i < ydata.size(); i++){
      PyList_SetItem(y, i, PyFloat_FromDouble(ydata[i]));
    }

    std::string fmt (iter->first.begin(), iter->first.end()-1);

    args = PyTuple_Pack(1, PyString_FromString(name.c_str()));
    PyObject_Call(figure, args, NULL);
    figures.push_back(name);
    Py_XDECREF(args);

    args = PyTuple_Pack(3, x, y, PyString_FromString(fmt.c_str()));
    PyObject_Call(plot, args, NULL);
    Py_XDECREF(args);

    Py_CLEAR(x);
    Py_CLEAR(y);

  }


  Py_XDECREF(args);

}

void Plotter::Plot(std::vector<double> ydata, std::string name, std::string fmt) {

  std::map<std::string, std::tuple<std::vector<double>, std::vector<double>>> data;

  data[fmt+"1"] = std::make_tuple(linspace(0, ydata.size(), ydata.size()), ydata);

  Plot(data, name);
}

void Plotter::Show() {

  PyObject *args;

  args = PyTuple_Pack(0);
  PyObject_Call(show, args, NULL);

  Py_XDECREF(args);
}

void Plotter::Save(std::string savedir, std::string which, std::string format) {
  PyObject *args = PyTuple_Pack(0);

  auto found = std::find(figures.begin(), figures.end(), which);

  if (found != figures.end()) {
    //Pick the plot to save - pull up that figure
    args = PyTuple_Pack(1, PyString_FromString(which.c_str()));
    PyObject_Call(figure, args, NULL);

    if ((*savedir.end()) == '/'){
      args = PyTuple_Pack(1, PyString_FromString((savedir+(*found)+format).c_str()));
      PyObject_Call(savefig, args, NULL);
    }
    else{
      args = PyTuple_Pack(1, PyString_FromString((savedir+"/"+(*found)+format).c_str()));
      PyObject_Call(savefig, args, NULL);
    }
  }
  else if (found == figures.end() and which != ""){
    std::cout << "Plot not found!  Double check your selection." << std::endl;
  }
  else{
    for (auto iter = figures.begin(); iter != figures.end(); iter++){

      args = PyTuple_Pack(1, PyString_FromString((*iter).c_str()));
      PyObject_Call(figure, args, NULL);

      if ((*savedir.end()) == '/'){
        args = PyTuple_Pack(1, PyString_FromString((savedir+(*iter)+format).c_str()));
        PyObject_Call(savefig, args, NULL);
      }
      else {
        args = PyTuple_Pack(1, PyString_FromString((savedir+"/"+(*iter)+format).c_str()));
        PyObject_Call(savefig, args, NULL);
      }
    }
  }
  Py_XDECREF(args);
}






