import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model




##==== global data
data_x = []
data_y = []




##==== sub-routines
## single point gradient
def dev_cal(index, k_learn, b_learn):
	global data_x
	global data_y

	num_point = len(data_x)
	x = data_x[index % num_point]
	y = data_y[index % num_point]

	dev_k = 2 * (k_learn * x + b_learn - y) * x
	dev_b = 2 * (k_learn * x + b_learn - y)

	return (dev_k, dev_b)


## stochastic gradient
def dev_cal_batch(k_learn, b_learn):
	global data_x
	global data_y

	dev_k = 0
	dev_b = 0
	size_batch = 50						# TODO: the batch size in stochastic learning

	list = np.random.permutation(num_point)
	dev_k = np.average( np.multiply( 2 * (k_learn * data_x[list[:size_batch]] + b_learn - data_y[list[:size_batch]]), data_x[list[:size_batch]] ) )
	dev_b = np.average( 2 * (k_learn * data_x[list[:size_batch]] + b_learn - data_y[list[:size_batch]]) )

	""" more readable
	num_point = len(data_x)
	list = np.random.permutation(num_point)
	for i in range(size_batch):
		index = list[i]
		x = data_x[index]
		y = data_y[index]
		dev_k += 2 * (k_learn * x + b_learn - y) * x
		dev_b += 2 * (k_learn * x + b_learn - y)
	dev_k = dev_k / size_batch
	dev_b = dev_b / size_batch
	"""

	return (dev_k, dev_b)


## total error
def error_cal(k_learn, b_learn):
	global data_x
	global data_y

	return np.sum( np.power(k_learn * data_x + b_learn - data_y, 2) )





if __name__ == "__main__":

	##===============================
	##==== the model
	##===============================
	k = 3.0							# TODO: real slope
	b = 100.0						# TODO: real intercept


	##===============================	
	##==== simulate data
	##===============================
	data_x = []						# list of tuples
	data_y = []						# list of float

	num_point = 1000					# TODO: number of point
	for i in range(num_point):
		x = 100 * np.random.random_sample()
		y = np.random.normal(k * x + b, 15)		# fixed Gaussian variance (--> based on visually how points are distributed)
		data_x.append(x)
		data_y.append(y)
	data_x = np.array(data_x)
	data_y = np.array(data_y)

	# save data
	file = open("./data_x.txt", "w")
	for i in range(len(data_x)):
		x = data_x[i]
		file.write(str(x) + "\n")
	file.close()
	file = open("./data_y.txt", "w")
	for i in range(len(data_y)):
		y = data_y[i]
		file.write(str(y) + "\n")
	file.close()



	##===============================	
	##==== GD1: int and sampling
	##===============================
	print "gradient descent (single round or batch-learning)"
	k_learn = 10 * np.random.random_sample()
	b_learn = 200 * np.random.random_sample()
	print "init k and b:", k_learn, b_learn

	error_list = []
	iter = 2000						# TODO: number of iterations to train
	alpha = 0.000001					# TODO: learning rate
	for i in range(iter):
		print "training iter#", i

		# gradient descent
		#dev_k, dev_b = dev_cal(i, k_learn, b_learn)
		dev_k, dev_b = dev_cal_batch(k_learn, b_learn)

		k_learn = k_learn - alpha * dev_k
		b_learn = b_learn - alpha * dev_b

		# error of this round
		error = error_cal(k_learn, b_learn)
		error_list.append(error)

	print "learned model:", k_learn, b_learn



	##===============================
	##==== GD2: scikit learn
	##===============================
	print "gradient descent (scikit learn)"
	data_x = data_x.reshape(-1, 1)
	regr = linear_model.LinearRegression()
	regr.fit(data_x, data_y)
	print "learned model:", (regr.coef_)[0], regr.intercept_




	##===============================
	##==== visualizing results
	##===============================
	## errors
	x = np.arange(iter)
	plt.plot(x, error_list)
	plt.xlabel("iterations")
	plt.ylabel("sum of squared errors")
	plt.title("Training errors v.s. Iterations")
	plt.show()


	## models
	# data
	plt.plot(data_x, data_y, 'go', alpha=0.2, label="data")

	x = np.arange(100)
	# simulator
	y = k * x + b
	plt.plot(x, y, 'r-', label="simulator")
	# gd1
	y = k_learn * x + b_learn
	plt.plot(x, y, 'b-', label="testing algorithm")
	# gd2
	y = (regr.coef_)[0] * x + regr.intercept_
	plt.plot(x, y, 'y-', label="Scikit-learn LinearRegression")
	plt.legend(loc=4)
	plt.xlabel("x")
	plt.ylabel("y")
	plt.title("Data, simulator and learned models")
	plt.show()




