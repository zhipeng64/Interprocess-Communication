**DISCLAIMER**

This project and its contents are intended strictly for educational and cybersecurity research purposes only. 
Unauthorized modification, injection, or exploitation of software is strictly prohibited. 
The author is not responsible for any misuse, damages, legal consequences, or losses resulting directly or indirectly 
from the use of this software. Use at your own risk.

**Summary**

This is a project that spawns a local process that injects and executes a custom DLL module
into a local target process. The local process presents a user interface for the user to enter the local
target process's name to inject to. 

**How to use it**

Clone the repository using Visual Studio. 

Build each of the projects, replacing the DLL filename placeholder with the DLL module you wish to use.

Run the .exe file, click on the Process Name tab to enter the name of the local process you wish to inject the DLL to and click "Ok".

The DLL will be injected and executed automatically.
