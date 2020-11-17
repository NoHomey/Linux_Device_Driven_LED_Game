# linux-device-driven-4-players-2048-clone
Repository for a school homework for the **Embedded systems programming practice** course in ***ELSYS (TUES) - Technological shcool "Electronic systems"***.

The task for the homework was to:

 1. Create a game of your choice based on modern embedded system;
 2. Make it distributed (split the input from the output);
 3. Choose the communication hardware interface: it needs to be wired comunication (Ethernet port is not allowed) and it should be self configured and only standart file operations for kernel modules are allowed for controlling the interface;
 4. Both the input and the output should be kernel based (device driver for each should be written);
 5. The input should be interrupts driven driver;
 6. The output should be classic device driver (`ioctl` may be used but it shouldn't be the core of the driver);
 7. Kernel modules should be configurable and should have a default configuration (it should be posssible to change the hardware wiring and the drivers should function without the need to change their source code);
 8. The game should be playable at the moment of project presentation;
 9. Homework should be done by two students and source controll should be used;
 10. There should be project separation and the repository should contain AUTHORS.md describing what each student did.

The homework result is:

 1. As a game a unique clone of the popular 2048 game has been made and Raspberry Pi 2 was chosen as an embedded system platform;
 2. ✓
 3. Serial (UART) hardware interface has been chosen as beeing the easiest to use from (I2C, SPI and UART);
 4. ✓
 5. ✓
 6. ✓
 7. ✓
 8. ✓
 9. ✓
 10. ✓

Project structure:

 - game - contains the game logic code
 - lib - contains device drivers configuration and controlling code
 - kernel_modules - contains the kernel modules source code
 - debug - contains code for debuging comunication between the two RPIs

Homeworks was done by me \<Ivo Stratev @NoHomey\> and \<Borislav Stratev @naganu\>.
