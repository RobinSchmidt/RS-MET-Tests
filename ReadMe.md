This repository is for testing code from the RS-MET codebase on real-world data.
Data, in this context, means mostly audio samples. I don't want to include 
large amounts of test-data into the main RS-MET repo, that's why the data-based
tests go into this extra repo. The main RS-MET codebase has also a lot of 
tests included already - but there, only those sorts of tests which don't need 
external data are included.

If you want to use the code, you need to put the main RS-MET folder/repo right 
next to the RS-MET-Tests folder/repo. The source and project files find the 
required files from the main repo via relative paths.