#### Making Array & Matrix
```python
array = np.array([...])
matrix = np.array([[],[],...])
matrix = np.mat([[],[],...])  # Not recommended since most np fxns return arrays
np.eye(x) # Identity Matrix of degree n
np.zeroes(x,y) # zeroed out matrix of x * y
np.ones(x,y)

```

#### Sparse matrix
```python
from scipy import sparse
mat = np.array([0,0,1],[1,0,0],[0,0,1])
sparse_mat = sparse.csr_matrix(mat) # Storres only non zero values and their indices saves space...when fed to regression models generates same result as normal mat but is more space and time efficient.
```

#### Indexing and slicing
```python
array = ....
array[i]
array[i][j]
array[x:y] # x inclusive and y is exclusive
array[:]  # generates a copy
array[-z] # negative indexing
array[a:b,c:d] # a to b(exclusive) rows and c to d(exclusive) cols
```

#### Array/Matrix detail Operations
```python
arr..shape   #(3,4) (Row and cols of matrix)
arr.size     # 12 (Total no of elems)
arr.ndim    # 2(No of dimensions)
arr.max()
arr.min()
```

#### Applying fxns to arrays
```python
func = lambda x: x*100
vectorized_func = np.vectorize(func)  # Making the fxn vector friendly
vectorized_func(mat)  # inputting mat to the func
mat * 100 # Same result
```

#### Numpy operations on arrays
```python
np.max(arr,params...) # AXIS - 0 for colwise and 1 for rowise
np.min(arr)
np.mean(arr)
np.var(arr)
np.std(arr)
q1,q3 = np.quartile(x,[25,75]) # Return the quartile values of a feature
np.where(cond..)  # Return a numpy array which satisies the condition.
```

#### Reshaping arrays ,diagonals and traces
```python
arr.reshape(3,-1) # -1 means as many as we need
arr.diagonal(offset = x) # get diagonal offsetted by x if -tve then lower diagonal
arr.trace()
```

#### Transposing & Flattening
```python
arr.T 
arr.flatten()  # makes into a 1d array
```


#### Rank, inverse and determinant
```python
np.linalg.matrix_rank(matrix)
np.linalg.inv(matrix)
np.linalg.det(matrix)
```

#### Dot product
```python
np.dot(a,b)
a @ b
```

#### Random values
```python
# Set seed
np.random.seed(0)
# Generate three random floats between 0.0 and 1.0
np.random.random(3)
# Generate three random integers between 1 and 10
np.random.randint(0, 11, 3)
# Draw three numbers from a logistic distribution with mean 0.0 and scale of 1.0
np.random.logistic(0.0, 1.0, 3)
# Draw three numbers greater than or equal to 1.0 and less than 2.0
np.random.uniform(1.0, 2.0, 3)
```

#### Eigon values and Eigon Vectors
```python
eigenvalues, eigenvectors = np.linalg.eig(matrix)
```



