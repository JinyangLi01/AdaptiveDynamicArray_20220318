# Adaptive Dynamic Array
 
## Abstract
Ordering data is important for interactively exploring and manipulating data. When accessing data on a tabular interface, users need to order tuples, to allow categorizing, comparing, and focusing on data of interest. Such order awareness for tabular data is
fundamentally missing in relational databases. This paper studies
ordered access for relational data. First, we propose the Ordered
Access Manager (OAM) which enables order-aware operations at a
frontend with minimum latency to access the underlying database.
Second, at the core of OAM, we propose a theoretically optimal solution, adaptive dynamic array (ADA), for indexing the rank-to-row
mapping. This index structure is novel– as a simple combination of
array and B-tree– and optimal in a change-aware manner, which
pays overhead only when more changes to the orders occur. Our ex-
periments validated that ADA can efficiently support ordered data
access from read-only to write-mostly scenarios with significant
performance margin from baselines.
