# TO-DO/FIX LIST
    ### LEGEND:
    #### [X] = DONE
    #### [~] = ALMOST DONE
    #### [...] = SOMEDAY
	#### [O] = SCRAPPED

DONE:
 [X] Fix main screen output when doing the math. (since the first operation leads to "0" printed).
 [X] Operation symbol problem 1. Changing between multiple consequetively fix-sets output to 0.
 [X] Operation symbol problem 2. first operation messes up eqation when putting 2 consequitive operations.
 [X] Operation symbol problem 3. using "*" or "/" after other operation causes problems.
 [X] Don't input/remove excessive/hanging operation from equation when pressing <Enter>.
 [X] Don't allow operation to be inserted first.
 [X] Fix problem: <Enter> causes program crash @ line 197. "HISTORY.insert(EQUATION);"
  ^  Fixed through implementing deep copy, unsure if that was the cause or not.
 [X] Figure out what to do with "=" in equation.
 [X] Fix <BACKSPACE> equation problem.
 [X] Don't allow equation preview (and anything else) to delete more than it should [only up to latest operation symbol].
 [X] Fix crash due to <Backspace> <Delete> (in either order) being inputted.
 [X] Not allow consequitive operations to be inputted "+--+/". Show last operation symbol replaced by new.
 [X] Add history.
  ^  I believe history is being saved.
 [X] <ESCAPE> opens options.
 [X] Parse HISTORY data into history screen.

IN PROGRESS:
 [~] Add [+/-] button.
 [ ] Fix scrollbarwidget in History.
 [ ] Single number entry causes program crash.
 [ ] Set continuation after pressing <ENTER>.
 [ ] Don't allow negation to final answer (unless I do ^)
 [ ] Negation after symbol input causes problem. -> Don't allow negation after symbol.
 [ ] Make system where you can erase symbols and get to numbers where you can edit and replace their values on the equation dLL.
 [ ] Add ability to change visuals (colors, mainly).
 [ ] Add history memory management (how much to store into history, basically).
 [ ] Edit size or scroll on output screen if number too long.
 [ ] Implement quick solving.
 [ ] Dynamically create new QPushButton in scrollarea for History when inserting into it.
 [~] Fix History not enabling when o/c.
 [ ] Save * location of EQUATION to recall it.
 [ ] Enable and fix scaling during resizing window.

SCRAPPED: