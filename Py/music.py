
#!/usr/bin/python3

class NoteInstruction:
    instruction_type = 1
    instrument = 0
    note = 0
    modifiers = 0
    start = 0
    duration = 0
    
    def __init__(self, note = 70, instrument = 0, start = 0, 
                 duration = 0, instruction_type = 1, modifiers = 0):
        self.note = note;
        self.instrument = instrument;
        self.start = start;
        self.duration = duration;
        self.instruction_type = instruction_type;
        self.modifiers = modifiers;
    
    def __str__(self):
        a = str(self.instruction_type) + ' ' + str(self.instrument) + ' ' + str(self.note) + ' ' + str(self.modifiers) + ' ' + str(self.start) + ' ' + str(self.duration)
        return a
        
class Music:
    def __init__(self):
        self.tape = []
        self.samplerate = 44100;
        self.beat_length = 1.0;
    
    def add(self, note = 70, instrument = 0, start = 0, 
                 duration = 0, instruction_type = 1, modifiers = 0, instruction = None):
        if(instruction == None):
            self.tape.append(NoteInstruction(note, instrument, start, duration, instruction_type, modifiers))
            return
        self.tape.append(instruction)
        
    def beat(self, note = 70, instrument = 0, start = 0, 
                 duration = 0, instruction_type = 1, modifiers = 0):
        self.tape.append(NoteInstruction(note, instrument, int(start*samplerate*beat_length), duration, instruction_type, modifiers))
    
    def __str__(self):
        a = ''
        for i in self.tape:
            a += str(i) + '\n'
        return a
