#### Configuring Matplotlib
```python
%matplotlib inline # Setting grids in graph in Jupyter notebook
%matplotlib notebook # No need to call plt.show().
import matplotlib.pyplot as plt
```

#### Parameters of plot
```python
plt.plot(marker='x') # sets datapoint markers as x
plt.plot(alpha=0.2) # sets transparency of points...Really godd for very dense data since it will show a contour map for dense.
plt.plot(xlabel="",ylabel="") # sets labels/name for x and y axis


```