#### Loading some common datasets from scikit
```python
from sklearn import datasets
x = datasets.(what you want to import)
x.data     # features data
x.target   # target vector
```

#### Creating simulated datasets
```python
# Regression

from sklearn.datasets import make_regression
# Generate features matrix, target vector, and the true coefficients
features, target, coefficients = make_regression(n_samples = 100, n_features = 3, n_informative = 3, n_targets = 1, noise = 0.0, coef = True, random_state = 1)
# No of samples,no of features/dependent variables,no of the features that affect the target out of total features, no of targets,noise,flag to also return coeffs/bias/weights , random state

#Classification
from sklearn.datasets import make_classification
# Generate features matrix and target vector
features, target = make_classification(n_samples = 100,n_features = 3, n_informative = 3,n_redundant = 0,n_classes = 2,weights = [.25, .75], random_state = 1)

# n_Redundant  helps simulate linear dependency/relation between the features,no of classes,weights based on how much data belongs to each class in data points

# Clustering
from sklearn.datasets import make_blobs
# Generate feature matrix and target vector
features, target = make_blobs(n_samples = 100,n_features = 2,centers = 3, cluster_std = 0.5,shuffle = True,random_state = 1)

# No of centers/clusters generated,spreadness of the cluster,shuffle is to randomly generate each cluster
```

#### Feature scaling
```python
from sklearn import preprocessing
fitter1 = preprocessing.MinMaxScaler(feature_range=(0,1)) # min max scaling [(x - min)/(max - min)] Also termed as Normalisation.
fitter2 = preprocessing.StandardScaler()  # Z-score scaling   Mean = 0 and stddev = 1        [(x-mean)/stddev]
fitter3 = preprocessing.RobustScaler() #If majority of data has outliers we prefer this
res = fitter.fit_transform(x) # Feature scale x
```

#### Normalization
```python
from sklearn.preprocessing import Normaliser
n1 = Normalizer(norm="l2")
#l1(Manhattan Norm) - Adds up magnitudes of features in single tuple of features
#l2(Root of sum of squares)
#l3(Max of all features in a tuple)
#lp(Similar to l2 but instead we raise by p and take root p of sum)
ans = n1.transform(x)

```


#### Applying Functions to dataset
```python
from sklearn.preprocessing import FunctionTransformer
def add_ten(x):
	return x + 10
p = FunctionTransformer(add_ten)
p.transform(x)

# Similar to pandas df.apply(fxn) method.
```


### Discretisation/Binning
```python
from sklearn.preprocessing import Binarizer

bin = Binarizer(x) # Makes each bin of size x
np.digitize(age, bins=[20,30,64]) #Filtering by binning in a fix set of bins



```