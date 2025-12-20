
# =========================
# Logistic Regression (sklearn)
# Pima Indians Diabetes
# =========================

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, roc_auc_score, confusion_matrix

# -------------------------
# Load dataset
# -------------------------
data = pd.read_csv("~/csv/pima-indians-diabetes.csv")

data.columns = [
    "Pregnancies",
    "Glucose",
    "BloodPressure",
    "SkinThickness",
    "Insulin",
    "BMI",
    "DiabetesPedigreeFunction",
    "Age",
    "Outcome"
]

# -------------------------
# Fix missing-value zeros
# -------------------------
cols = ["Glucose","BloodPressure","SkinThickness","Insulin","BMI"]
data[cols] = data[cols].replace(0, np.nan)
data[cols] = data[cols].fillna(data[cols].median())

# -------------------------
# Split features / target
# -------------------------
X = data.drop(columns="Outcome")
y = data["Outcome"]

X_train, X_test, y_train, y_test = train_test_split(
    X, y,
    test_size=0.2,
    random_state=42,
    stratify=y
)

# -------------------------
# Feature scaling
# -------------------------
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# -------------------------
# Logistic Regression model
# -------------------------
clf = LogisticRegression(
    penalty="l2",
    C=1.0,           # inverse regularization strength
    solver="lbfgs",
    max_iter=1000
)

clf.fit(X_train, y_train)

# -------------------------
# Evaluation
# -------------------------
y_pred = clf.predict(X_test)
probs = clf.predict_proba(X_test)[:, 1]

print("Accuracy:", accuracy_score(y_test, y_pred))
print("ROC-AUC :", roc_auc_score(y_test, probs))

print("\nConfusion Matrix:")
print(confusion_matrix(y_test, y_pred))

