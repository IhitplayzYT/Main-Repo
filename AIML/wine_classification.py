#@
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.datasets import load_wine
#@
data = load_wine()
x,y = data.data, data.target
df = pd.DataFrame(data=x,columns=data.feature_names)
df["target"] = y


X,X_t,Y,Y_t = train_test_split(x,y,random_state=42,test_size= 0.2,stratify=y)
Y = np.array(Y).reshape(-1,1)
Y_t = np.array(Y_t).reshape(-1,1)

mu = X.mean(axis=0)
sig = X.std(axis=0)
X = (X - mu) / sig
X_t = (X_t - mu) / sig

X = np.c_[np.ones((X.shape[0],1)),X]
X_t = np.c_[np.ones((X_t.shape[0],1)),X_t]

def softmax(z):
    z = z - np.max(z, axis=1, keepdims=True) 
    exp_z = np.exp(z)
    return exp_z / np.sum(exp_z, axis=1, keepdims=True)

def one_hot_encode_basic(y, num_classes=None):
    y = y.flatten().astype(int)
    if num_classes is None:
        num_classes = np.max(y) + 1
    return np.eye(num_classes)[y]
#@ Regression
m = len(df["target"].unique())
n = X.shape[0]
M = np.random.randn(X.shape[1],m) * 0.01
EPOCH,eps = 10000,1e-8
LR,ALPHA = 0.001,0.01
Y_oh = one_hot_encode_basic(Y,3)
for i in range(EPOCH):
    prob = softmax(X @ M)
    loss = - np.sum(Y_oh * np.log(prob+eps))/n 
    grad = X.T @ (prob - Y_oh) / n
    M -= LR * grad
    if not i%100:
        print(f"Loss {i} : {loss}")


y_pred = np.argmax(softmax(X_t @ M),axis= 1)
print(f"Acc = {np.mean(y_pred == Y_t.flatten())}")

