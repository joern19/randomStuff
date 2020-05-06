class logicClass:

    game = [[None,None,None],[None,None,None],[None,None,None]]
    #1 -> X
    #2 -> O

    def checkwin(self, player, x, y):

        if (self.game[0][y] == self.game[1][y] == self.game[2][y]):
            return True

        if self.game[x][0] == self.game[x][1] == self.game[x][2]:
            return True

        if x == y and self.game[0][0] == self.game[1][1] == self.game [2][2]:
            return True

        if x + y == 2 and self.game[0][2] == self.game[1][1] == self.game [2][0]:
            return True
        return False

    def place(self, h, v, player):
        if self.game[h][v] == None:
            self.game[h][v] = player
            if self.checkwin(player, h, v):
                return None
            return True
        else:
            return False