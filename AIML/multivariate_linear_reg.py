#@ Imports
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
#@ Read Data and cleaning
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/wine-quality/winequality-red.csv"
data = pd.read_csv(url, sep=";")

#@ Looking at data
X = data.drop(columns=["quality","residual sugar","free sulfur dioxide","pH"])  
# Very less corr hence eliminating noise
X = (X - X.mean(axis=0)) / X.std(axis=0)
n = X.shape[0]
Y = np.array(data["quality"]).reshape(-1,1)
X = np.c_[np.ones((n,1)),X]
#@
M = np.random.randn(X.shape[1],1)
EPOCH = 10000
ALPHA = 0.00025
LAMBDA = 0.62
for i in range(EPOCH):
    y_err = Y - X @ M
    grad = -(2/n) * (X.T @ y_err)
    reg = 2 * LAMBDA * M
    reg[0] = 0
    M = M - ALPHA * (grad + reg)
    if not i % 100:
        print(f"ERR at {i}th iter : {np.sum(y_err ** 2)/n}")


r2 = 1 - (np.sum((Y - X @ M) ** 2)) /  (np.sum((Y - Y.mean()) ** 2)) 
print(f"R2 err = {r2}")

