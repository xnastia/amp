How to run  the app

1. Win7.  VisualStudio 2013. 
Install Cereal library
http://uscilab.github.io/cereal/quickstart.html
Copy cereal folder to %VSPath%/VC/include
2. new project console  C++ app ->empty
3. Set properties for project
   
General tab.   Whole Program optimization -> Use Link Time code generation
C++ General tab. Debug information -> C7

4. If you want to run from VS editor in debug mode. Set path to input file. Now it point to
C:\\simulate_json\Data0.json