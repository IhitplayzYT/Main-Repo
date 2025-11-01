#### Reading from file streams and creating dataframes
```python
df = pd.read_csv(r'') #r'' is a raw string ie \ is not escapped or \\ can be used in the string instead, read_.. can also read rrom urls

df = pd.Dataframe() 
df['x'] = [...]  # adds a new col to df
...

# Create row
new_person = pd.Series(['Molly Mooney', 40, True], index=['Name','Age','Driver'])  # similar to a list just that each index is labelled here.
```


#### Merging,concat and appending
```python
df.append([])  # adds a tuple to the df
dataframe = dataframe.set_index(dataframe['Name']) #Making one of the columns as the index of the df
pd.concat([],[],..,axis) # axis = 0 conacts along rows and 1 concats along col.
pd.merge([],[]...,on,how) # Decide which row to merge on ,how tells which merge to do left,right,inner and outer.





```

#### Dataframe Details
```python
df.head(x) # first x rows 10 is default
df.tail()
df.describe() # gives count,min,max,quartiles,mean ans std info for numerical cols.
df.shape
df['x'].min()
df['x'].max()
df['x'].sum()
df['x'].mean()
df['x'].count()

```

#### Dataframe Handling
```python
# Select first row
df.iloc[0:] # Based on col and row index
df.loc['x'] #Based on col name and row index
df[(df['x'] == y) | (df['x'] == z) & ...]  # Conditional selection (Note and and or donoted by & and |)
dataframe['Sex'].replace(["female", "male"], ["Woman", "Man"])  # REplaces all instance of female and male in sex col with woman and man
dataframe.rename(columns={'PClass': 'Passenger Class'})  # Renaming columns

dataframe.drop('Age', axis=1)  # Drop a column
dataframe.drop([0,1], axis=0)  # Drop rows

```


#### Dataframe interaction
```python
df['x'].unique() # returns all unique values in col x
df['x'].value_counts() # return the value and freq of the values in col x
df['x'].nunique() # Mno of unique values in x
df.group_by('x')['y'].(Some operation we need to apply) # groups by 'x' similar to sql where groupby has to use a aggregate fxn or some fxn.
df.apply(func)  # Applies the fxn to a df or a col
df.group_by('x').apply(lambda x: x.count())[0:2] # Applies fxn for rows 0 to 1

```

**NAN not natively supported by pandas hence to use it we need np.nan**

#### Time
```python
time_index = pd.date_range('06/06/2017', periods=100000, freq='30S')  # creates a array with time starting from 06/06/2017 for 30 second intervals for 100000 times.

df.resample('2W') # Helps group data by time periods here 2 week groups made
```

#### Dataset cleaning
```python
df[df['x'.isnull()]]  # return all tuples with x as null
df.drop_duplicates() # drops duplicate tuples
df['x'].drop_duplicates() # drops tuples of duplicate x
dataframe.drop_duplicates(subset=['Sex'],keep='last')  # same as above but deletes all duplicates except last.








```
