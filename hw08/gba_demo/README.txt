1. Light up a pixel
2. Light up a pixel in the center of the screen
3. Change the pixel color
4. Draw a rectangle
5. Bounce a rectangle -- without undrawing
6. Bounce a rectangle -- undrawing but without delay
7. Bounce a rectangle -- with delay and undrawing
8. Get the bouncing boundaries right

>>>>Discuss video blanking in L17
9. Bounce a rectangle -- with vblank delay and undrawing

>>>>Discuss buttons in L17
10. Use up-arrow and down-arrow to change the size — issues with holding key
11. Fix button behavior
12. Add state
13. Add multiple balls
14. Allow dynamic ball count changes

>>>>Discuss DMA in L17
15. Add DMA
16. Split off library (add lib.o to OFILES in Makefile)

>>>>Discuss text in L17
17. Add scoring (add text.o font.o to OFILES in Makefile)
18. Add splash screen and game over screen (probably left to the student to read)

Compile instructions:
Copy the version of interest to main.c
For versions 16-18, add the appropriate .o files to the Makefile
Run "make mgba"
