# TO-DO/FIX LIST
     LEGEND:
    [X] = DONE
    [#] = ALMOST DONE
    [...] = SOMEDAY
	[O] = SCRAPPED

## DONE:

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
 
 [X] Dynamically create new QPushButton in scrollarea for History when inserting into it.
 
 [X] Make settings screen a stacked widget.
 
 [X] Don't allow multiple periods.
 
 [X] Add decimal places.
 
 [X] Single number entry causes program crash.
 
 [X] Make "settings button" a navigation button (switch text to <-) when not in main settings screen.
 
 [X] Add [+/-] button.
 
 [X] Don't allow leading zeroes.
 
 [X] Set continuation after pressing <ENTER>.

 [X] Don't allow operation after "-" && negated
 
 [X] Fix backspace issues when the number is a negative.
 
 [X] Fix scrollbarwidget in History.
 
 [X] Add limit on amount of #'s allowed to be inputted.
 
 [X] Resize on output screen if number too long.
 
 [X] Fix problem when removing from HISTORY. (Has to do with inserting a dLL into it being deconstructed right after the function call...)  
 
 [X] Fix crash at closing program w/ multiple arithmetics. (due to button closure in history class)	
 
## IN PROGRESS:
 
 [ ] Implement history recall.
    
 [ ] Negating answer, then pressing digit makes equation screen wrong.
   
 [ ] Redo insertion into History by using an actual deep copy constructor.
   
 [ ] Fix and enable percent, inverse, sqrt, and power.
 
 [ ] Fix program scaling (through px/em, and compatability with different resolutions).
  
 [ ] Add an <ans> button to recall latest answer.
 
 [ ] Add horizontal scrollability to equations.
 
 [ ] Add .rc/version & details file
 
 [ ] Plug any memory possible leaks.
 
 [ ] Make program more lightweight. (remove unnecessary comparisons, fix logic, etc.) 
   
## SCRAPPED IDEAS:

 QS Function (QuickSolve) removed. [Reason: to make app better looking (and implementation would've maybe taken too long, might try once more later.)]
 
 History memory management (how much to store into history, basically). [Reason: Not neccessarily needed, idea was just to have more stuff in program.]

 Dynamic system where the user can erase all they want and redo anything anywhere. [Reason: Would take too much time and not really necessary. Might add/do in the future.]

 Ability to change visuals (colors, mainly).
 
# NOTES:

 History icon by Hadrien from www.flaticon.com
 
 Back icon by Lucy G from www.flaticon.com
 
 Settings icon by Smashicons from www.flaticon.com