radio
=====

spectrum builder for USRP-N210
scans the range of wavelengths (tested on 100-10 meter waves) following a certain LFM signal with a predefined constant speed. builds a spectrogram. 

total amount of collected data does not depend on the computer's qualities. building a spectrogram may take a while, though.

syncs the USRP by its PPS. the time on computer shouldn't deviate by more than one second off transmitter's time.

uses the matched filter method.
