# EscorpioEvo16 LogBug
##Introduction
This software is used to parse and show the log message sent from the [EscorpioEvo16-Dashboard](https://github.com/DavideMalvezzi/EscorpioEvo16-Dashboard) 
with the Logger module.
Each message has:
- the receive date and time;
- the module tag (a short string that identify which module has sent the message);
- the message itself.

All these information are shown with different color according the message type (red for error message, yellow for warnings and white for 
normal message).
The LogBug software can also filter the messages by the tags and also show/hide the messages by the type.
