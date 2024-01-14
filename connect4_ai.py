import copy 

OPPONENT = -1
AI = 1

FIRST_PLAYER = None
SECOND_PLAYER = None

board = [[0, 1, 1,-1, 1,-1, 0],
	     [0, 1, 0, 1,-1, 0, 0],
	     [0,-1, 0,-1, 1, 0, 0],
		 [0,-1, 0,-1,-1, 0, 0],
	     [0, 1, 0, 1, 1, 0, 0],
	     [0, 0 ,0, 0,-1, 0, 0]]
         
boards = [
		 [[0,0,0,0,0,0,0],
           [0,0,0,0,0,0,0],
           [0,0,0,0,0,0,0],
           [0,0,0,0,0,0,0],
           [0,0,0,0,0,0,0],
           [0,0,0,0,0,0,0]]
         ]

f = open("hashmaps_depth7.csv")
hashmaps = [[float(n) for n in i.split(",")] for i in f.read().split("\n")[0:-1]]

def valid_move(board, column):
	if column > 6:
		return False
	for i in board:
		if i[column] == 0:
			return True
	return False

def whose_turn(board):
	counter = 0
	for y in board:
		for x in y:
			if x == 0:
				counter += 1
	if counter % 2 == 0:
		return FIRST_PLAYER
	else: 
		return SECOND_PLAYER
		
def input_board(board,column, player):
	for i in board:
		if i[column] == 0:
			i[column] = player
			break
	return board

def to_int(board):
	ai_board = copy.deepcopy(board)
	opponent_board = copy.deepcopy(board)
	ai_int = ""
	opponent_int = ""
	for y in ai_board:
		for x in y:
			if x == -1:
				x = 0
			ai_int += str(x)
				
	for y in opponent_board:
		for x in y:
			if x == 1:
				x = 0
			if x == -1:
				x = 1
			opponent_int += str(x)			
	return int(ai_int, 2), int(opponent_int, 2)

def terminal(board):
	#horizontal
	for y in range(6):
		for x in range(4):
			if abs(sum(board[y][x:x+4])) == 4:
				return True
				
	#vertical
	for x in range(7):
		for y in range(3):
			if abs(sum([i[x] for i in board[y:y+4]])) == 4:
				return True	
	#diagonals
	for x in range(4):
		for y in range(3):
			if abs(sum(board[y+i][x+i] for i in range(4))) == 4 or abs(sum(board[::-1][y+i][x+i] for i in range(4))) == 4:
				return True
	counter = 0
	for y in board:
		for x in y:
			if x != 0:
				counter += 1
	
	if counter == 42:
		return True
	else:
		return False
		
def evaluate_segment(segment):
	global AI
	global OPPONENT
	score = 0
	if segment.count(AI) == 4:
		score = float("inf")
	if segment.count(AI) == 3 and segment.count(0) == 1:
		score += 5
	if segment.count(AI) == 2  and segment.count(0) == 2:
		score += 2
	if segment.count(OPPONENT) == 4:
		score = -float("inf")
	if segment.count(OPPONENT) == 3  and segment.count(0) == 1:
		score -= 5
	if segment.count(OPPONENT) == 2  and segment.count(0) == 2:
		score -= 2
	return score
	
def evaluate_position(board):
	score = 0
	
	for y in range(6):
		for x in range(4):
			score += evaluate_segment(board[y][x:x+4])
	for x in range(7):
		for y in range(3):
			score += evaluate_segment([i[x] for i in board[y:y+4]])
	for x in range(4):
		for y in range(3):
			score += evaluate_segment([board[y+i][x+i] for i in range(4)])
			score += evaluate_segment([board[::-1][y+i][x+i] for i in range(4)])
	return score

def minimax(board, player, depth, alpha, beta):
	#print(board)
	'''for y in range(6):
		for x in range(7):
			print("{0:+}".format(board[y][x]), end="")
		print("")
	print("-----" +str(depth)+"-------")'''
	global AI
	global OPPONENT
	global hashmaps
	
	'''if depth == 7:
		for i in hashmaps:
			if to_int(board) == tuple(i[0:2]):
				return i[3], int(i[2])'''
	
	if depth == 0 or terminal(board):
		return evaluate_position(board), None
	
	if player == AI:
		value = -float("inf")
		best_move = 0
		for column in range(0,7):
			if valid_move(board, column):
				board_copy = copy.deepcopy(board)
				board_copy = input_board(board_copy,column,AI)
				new_value,move = minimax(board_copy, OPPONENT, depth - 1, alpha, beta)
				alpha = max(value, alpha)
				if alpha >= beta:
					break
				if new_value > value:
					value = new_value
					best_move = column
					print("Best Move is :" + str(best_move+1) + "---->" + "{0:+}".format(value) if depth == 7 else "", end="\r")
		return value, best_move
		
	if player == OPPONENT:
		value = float("inf")
		best_move = 0
		for column in range(0,7):
			if valid_move(board, column):
				board_copy = copy.deepcopy(board)
				board_copy = input_board(board_copy,column,OPPONENT)
				new_value,move = minimax(board_copy, AI, depth - 1, alpha, beta)
				beta = min(value, beta)
				if alpha >= beta:
					break
				if new_value < value:
					value = new_value
					best_move = column
		return value, best_move
#Gameplay

def main():
	global OPPONENT
	global AI
	global board
	print(minimax(board, AI, 5, -float("inf"), float("inf")))
	'''while True:
		turn = input("Who Goes first?:")
		if turn == "me":
			FIRST_PLAYER = OPPONENT
			SECOND_PLAYER = AI
			break
		if turn == "you":
			FIRST_PLAYER = AI
			SECOND_PLAYER = OPPONENT
			break
	if turn == "me":
		while True:
			move = int(input("Make a move:"))
			if valid_move(board,move-1):
				board = input_board(board,move-1,OPPONENT)
				break
	
	score_log = []
	while True:
		value, ai_move = minimax(board, AI, 7, -float("inf"), float("inf"))
		print("")
		print("My move is: " + str(ai_move+1))
		print("The score predicted is: " + str(value))
		score_log.append(value)
		#print(to_int(board))
		board = input_board(board, ai_move, AI)
		for y in range(6):
			for x in range(7):
				print(("\033[93m X" if board[::-1][y][x] == 1 else "\033[31m O" if board[::-1][y][x] == -1 else "\033[97m  "),end="" )#+ "{0:+}".format(board[::-1][y][x]), end="")
			print("")
		print("\033[97m -------------")
		print(" 1 2 3 4 5 6 7")
		print("\033[0m")
		if terminal(board):
			print("Game over!")
			print("Score Log:", end="")
			for x in score_log:
				print(str(x) + " ", end="")
			break
		while True:
			move = int(input("Make a move:"))
			if valid_move(board, move-1):
				board = input_board(board, move-1, OPPONENT)
				break
			print("Invalid Move! Try Again")
		if terminal(board):
			print("Game over!")
			break'''


main()

def train(board):
	f = open("hashmaps_depth7(1).csv", "a")
	value, ai_move = minimax(board, AI, 7, -float("inf"), float("inf"))
	f.write(str(to_int(board)[0]) + "," + str(to_int(board)[1]) + "," + str(ai_move) + "," + str(value) + "\n")
	print(f"Best move is {ai_move+1}")
	return ai_move


def pt(board):
	for y in range(6):
		for x in range(7):
			print(("\033[93m X" if board[::-1][y][x] == 1 else "\033[31m O" if board[::-1][y][x] == -1 else "\033[97m  "),end="" )#+ "{0:+}".format(board[::-1][y][x]), end="")
		print("")
	print("\033[97m -------------")
	print(" 1 2 3 4 5 6 7")
	print("\033[0m")
 
def ok(t):
	t1= t[0]
	t2= t[1]
	b1 = str(bin(t1))[2::].zfill(42)
	b2 = str(bin(t2))[2::].zfill(42)
	import numpy as np
	b1 = [int(x) for x in b1]
	b2 = [int(x) for x in b2]
	b=[]
	for i in range(0,42):
		if b1[i] == 1:
			b.append(1)
		elif b2[i] == 1:
			b.append(-1)
		else:
			b.append(0)
	pt(np.array(b).reshape(6,7))

'''import time
ok(tuple(int(i) for i in hashmaps[5][0:2]))
for x in range(7):
	print("At Depth: " + str(x+1))
	c = 1
	boards_copy = copy.deepcopy(boards)
	for b in range(len(boards_copy)):
		pt(boards[b])
	
	for b in range(len(boards_copy)):
		if terminal(boards_copy[b]) == False:
			for i in range(7):
				if valid_move(boards_copy[b],i):
					pt(boards[b])
					boards.append(input_board(copy.deepcopy(boards_copy[b]),i,-1))
					pt(boards[-1])
					print(f"Dropped on {i+1}")
		boards.remove(boards[b])

	for b in range(len(boards)):
		print("Depth " + str(x+1) + " and Board " + str(c))
		pt(boards[b])
		move = 1#train(b)
		boards[b] = input_board(copy.deepcopy(boards[b]),move,1)
		pt(boards[b])
		c += 1
		
		time.sleep(0.1)
'''
