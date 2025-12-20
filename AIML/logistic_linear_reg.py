#@ Imports
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.datasets import load_breast_cancer
from sklearn.model_selection import train_test_split
#@ Loading dataset
data = load_breast_cancer()
X,Y = data.data,data.target
X_label,Y_label = data.feature_names,data.target_names
data = pd.DataFrame(X,columns=X_label)
data["target"] = Y

Y = np.array(data["target"]).reshape(-1,1)
X = data.drop(columns=["target"])
X_train, X_test, Y_train, Y_test = train_test_split(
    X, Y, test_size=0.2, random_state=42, stratify=Y
)

X_test = (X_test - X_train.mean(axis=0)) / X_train.std(axis=0)
X_train = (X_train - X_train.mean(axis=0)) / X_train.std(axis=0)
X_train = np.c_[np.ones((X_train.shape[0],1)),X_train]
X_test = np.c_[np.ones((X_test.shape[0],1)),X_test]
n = X_train.shape[0]
def sigmoid(x):
    return 1/ (1 + np.exp(-x))

#@ Logistic Reg
EPOCH,ALPHA,LAMBDA = 10000,0.03,1
M = np.random.randn(X_train.shape[1],1)
eps = 1e-8
for i in range(EPOCH):
    y_pred = sigmoid(X_train @ M)
    loss = - np.mean( Y_train * np.log(y_pred+eps) + (1 - Y_train)* np.log(1- y_pred+eps)) + np.sum(M[1:] ** 2) * LAMBDA/(2 * n)
    grad = - (1/n) * X_train.T @ (Y_train - y_pred)
    grad[1:] += (LAMBDA/n) * M[1:]
    M = M - ALPHA * grad
    if not i % 100:
        print(f"Loss in iter {i} : {loss}")



y_pred = (sigmoid(X_test @ M) >= 0.5).astype(int)

accuracy = np.mean(y_pred == Y_test)
print("Accuracy:", accuracy)








