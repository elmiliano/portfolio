import datetime 

# DATETIME
now = datetime.datetime.today()
f = '%Y-%m-%d %H:%M:%S'
now = now.strftime(f)

class RGB(): # define RGB class
    def __init__(self,r,g,b):
        self.r = r
        self.g = g
        self.b = b
        self.grade = None
        self.chargrade = None

    def dictgrade(self): # return RGB values and maturity
        return {
            'R': self.r,
            'G': self.g,
            'B': self.b,
            'Grade': self.grade,
            'Maturity': self.chargrade,
            'Moment' : now
        }
    
    def evaluate(self): # evaluate banana maturity from RGB value (NOT USED)
        if ((self.r > self.g) and ((self.r - self.g) < 9)) and (self.g >= 142):
            self.grade = 3
            self.chargrade = 'RIPE'
        elif ((self.r > self.g) and ((self.r-self.g) >= 9)) and (self.g >= 142):
            self.grade = 5
            self.chargrade = 'OVERRIPE'
        elif (self.r > self.g) and (self.g <= 142):
            self.grade = 7 
            self.chargrade = 'THROW AWAY'
        elif (self.g > self.b):
            self.grade = 1
            self.chargrade = 'NOT RIPE'
        return self.grade

    def evaluate_banana(self): # evaluate banana maturity from RGB value
        green_threshold = 100
        yellow_threshold = 200

        if self.g > self.r and self.g > self.b and self.g > green_threshold:
            self.grade = 1
            self.chargrade = 'NOT RIPE'
        elif self.r >= self.g >= self.b and self.g >= green_threshold and self.g < yellow_threshold:
            self.grade = 3
            self.chargrade = 'RIPE'
        elif self.r >= self.g >= self.b and self.g >= yellow_threshold:
            self.grade = 5
            self.chargrade = 'OVERRIPE'
        else:
            self.grade = 7
            self.chargrade = 'THROW AWAY'

        return self.grade

if __name__ == '__main__':
    banana = RGB(105,240,187)
    banana.evaluate()
    print(banana.grade)
    print(banana.dictgrade())

