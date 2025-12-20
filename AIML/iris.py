#@
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.datasets import load_iris
data = load_iris()
df = pd.DataFrame(data=data.data,columns=data.feature_names)
df["target"] = data.target
print(df.info())
Y = df["target"]
X = df.drop(columns=["target"])
X,X_t,Y,Y_t = train_test_split(X,Y,random_state=42,stratify=Y,test_size=0.2)
###
X = (X - X.mean(axis = 0)) / X.std(axis=0)
X_t = (X_t - X.mean(axis = 0)) / X.std(axis=0)
###
X = np.c_[np.ones((X.shape[0],1)),X]
X_t = np.c_[np.ones((X_t.shape[0],1)),X_t]

##
def softmax(v):
    v = v - np.max(v, axis=1, keepdims=True)  # for numerical stability
    exp_t = np.exp(v)
    return exp_t / np.sum(exp_t, axis=1, keepdims=True)
def one_hot_encode(y,l):
    y = y.flatten()
    return np.eye(l)[y]
#@
M = np.random.randn(X.shape[1],3) * 0.01
EPOCH,EPS = 10000,1e-8
LR = 0.01
n = X.shape[0]
Y = one_hot_encode(Y.values,3)
LAMBDA = 0.01
for i in range(EPOCH):
LR = LR * 0.95 if i % 100 == 0 else LR
loss += LAMBDA * np.sum(M**2) / (2*n)
grad += (LAMBDA * M) / n

# Mini-batch gradient descent
batch_size = 32
for batch in range(0, n, batch_size):
    X_batch = X[batch:batch+batch_size]
    Y_batch = Y[batch:batch+batch_size]
    # ... training on batch
    prob = softmax(X @ M)
    loss = (-np.sum(Y * np.log(prob+EPS)) / n)
    grad = (X.T @ (prob - Y)) / n 
    M = M - LR * grad
    if not i % 100:
        print(f"Loss {i} : {loss}")

y_pred = np.argmax(softmax(X_t @ M),axis= 1)
print(softmax(X_t @ M))
print(y_pred)








