# LeafLogger

## What can it do

A simple logger which can:

- Log messages with time and other information;
- Capture the messages passed to instances of `QDebug` (like `qDebug()`) and log them;
- Print messages to `stderr` and save them in a specific file cocurrently;
- Process messages asynchronously

## How to use it

First, go to release page to download the release, and decompress it in the folder you need.
LeafLogger is encapsulated in a shared library, so you need to configurate the include path and library path in your project. The "Add library" feature in Qt Creator may help.
To use the LeafLogger in your project is very easy. Just call `LeafLogger::LogInit()` in the `main()`. The method will create a log file named like "log20190628161255975.txt" in the "log" folder where your application works. Then it log the system infomation. And you can just continue to use the `qDebug()` or other instances of `QDebug` Qt provides. LeafLogger also provides its own method to log: `LogMessage(const QString& log)`.
Also, the class provides other methods to control the behavior. You can explore them in the source code.
