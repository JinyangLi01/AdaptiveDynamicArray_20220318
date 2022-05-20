cmake_minimum_required(VERSION 3.21)
project(AdaptiveDynamicArray_20220318)

set(CMAKE_CXX_STANDARD 14)

include_directories(DA_Bidirectional)
include_directories(DataStructures)
include_directories(Experiments/CaseStudy/workload/pooling)

#add_executable(AdaptiveDynamicArray_20220318
#        DA_Bidirectional/DAbidirectional.cpp
#        DA_Bidirectional/DAbidirectional.h
#        DA_Bidirectional/DAnolimitBidirectional.cpp
#        DA_Bidirectional/DAnolimitBidirectional.h
#        DA_Bidirectional/DAstaticBidirectional.cpp
#        DA_Bidirectional/DAstaticBidirectional.h
#        DataStructures/CountedBtree.cpp
#        DataStructures/CountedBtree.h
#        DataStructures/DAnolimit.cpp
#        DataStructures/DAnolimit.h
#        DataStructures/DAstatic.cpp
#        DataStructures/DAstatic.h
#        DataStructures/DynamicArray.cpp
#        DataStructures/DynamicArray.h
#        DataStructures/LinkedList.cpp
#        DataStructures/LinkedList.h
#        DataStructures/StandardArray.cpp
#        DataStructures/StandardArray.h
#        DataStructures/tiered-vector.h
#        Experiments/Ablation/100K/delete/average.cpp
#        Experiments/Ablation/100K/delete/delete.cpp
#        Experiments/Ablation/100K/insert/average.cpp
#        Experiments/Ablation/100K/insert/insert.cpp
#        Experiments/Ablation/100K/move/average.cpp
#        Experiments/Ablation/100K/move/move.cpp
#        Experiments/Ablation/100K/moveLength/move.cpp
#        Experiments/Ablation/100K/pointquery/levelchange.cpp
#        Experiments/Ablation/100K/pointquery/pointquery10.cpp
#        Experiments/Ablation/100K/pointquery/pointquery1000.cpp
#        Experiments/Ablation/100K/pointquery/pointquery10000.cpp
#        Experiments/Ablation/100K/pointquery/pointquery500.cpp
#        Experiments/Ablation/100K/pointquery/pointquery5000.cpp
#        Experiments/Ablation/100K/query/query.cpp
#        Experiments/Ablation/100K/queryLength/query.cpp
#        Experiments/Ablation/100K/reorder/reorder.cpp
#        Experiments/Ablation/100K/reorderLength/reorder.cpp
#        Experiments/Ablation/100K/swap/average.cpp
#        Experiments/Ablation/100K/swap/swap.cpp
#        Experiments/Ablation/100K/swapLength/swap.cpp
#        Experiments/Ablation/10K/delete/average.cpp
#        Experiments/Ablation/10K/delete/delete.cpp
#        Experiments/Ablation/10K/insert/average.cpp
#        Experiments/Ablation/10K/insert/insert.cpp
#        Experiments/Ablation/10K/pointquery/pointquery10.cpp
#        Experiments/Ablation/10K/query/query.cpp
#        Experiments/Ablation/10K/reorder/reorder.cpp
#        Experiments/Ablation/10K/swap/average.cpp
#        Experiments/Ablation/10K/swap/swap.cpp
#        Experiments/Ablation/1M/delete/average.cpp
#        Experiments/Ablation/1M/delete/delete.cpp
#        Experiments/Ablation/1M/insert/average.cpp
#        Experiments/Ablation/1M/insert/insert.cpp
#        Experiments/Ablation/1M/move/average.cpp
#        Experiments/Ablation/1M/move/move.cpp
#        Experiments/Ablation/1M/moveLength/move.cpp
#        Experiments/Ablation/1M/moveLength/move10.cpp
#        Experiments/Ablation/1M/moveLength/move2.cpp
#        Experiments/Ablation/1M/moveLength/move4.cpp
#        Experiments/Ablation/1M/moveLength/move40.cpp
#        Experiments/Ablation/1M/moveLength/move60.cpp
#        Experiments/Ablation/1M/pointquery/pointquery.cpp
#        Experiments/Ablation/1M/query/query.cpp
#        Experiments/Ablation/1M/queryLength/query.cpp
#        Experiments/Ablation/1M/reorder/reorder.cpp
#        Experiments/Ablation/1M/reorderLength/reorder.cpp
#        Experiments/Ablation/1M/swap/average.cpp
#        Experiments/Ablation/1M/swap/swap.cpp
#        Experiments/Ablation/1M/swapLength/swap.cpp
#        Experiments/Ablation/1M/delete.cpp
#        Experiments/Ablation/1M/insert.cpp
#        Experiments/Ablation/1M/move.cpp
#        Experiments/Ablation/1M/pointquery.cpp
#        Experiments/Ablation/1M/query.cpp
#        Experiments/Ablation/1M/reorder.cpp
#        Experiments/Ablation/1M/swap.cpp
#        Experiments/AblationOne+Bidirectional/100K/delete.cpp
#        Experiments/AblationOne+Bidirectional/100K/insert.cpp
#        Experiments/AblationOne+Bidirectional/100K/move.cpp
#        Experiments/AblationOne+Bidirectional/100K/query.cpp
#        Experiments/AblationOne+Bidirectional/100K/reorder.cpp
#        Experiments/AblationOne+Bidirectional/100K/swap.cpp
#        Experiments/AblationOne+Bidirectional/1M/delete.cpp
#        Experiments/AblationOne+Bidirectional/1M/insert.cpp
#        Experiments/AblationOne+Bidirectional/1M/move.cpp
#        Experiments/AblationOne+Bidirectional/1M/query.cpp
#        Experiments/AblationOne+Bidirectional/1M/reorder.cpp
#        Experiments/AblationOne+Bidirectional/1M/swap.cpp
#        Experiments/ADA_TV/insert/insert1.cpp
#        Experiments/ADA_TV/insert/insert2.cpp
#        Experiments/ADA_TV/insert/insert3.cpp
#        Experiments/AveBaseline/100K/0.cpp
#        Experiments/AveBaseline/100K/1.cpp
#        Experiments/AveBaseline/100K/2.cpp
#        Experiments/AveBaseline/100K/3.cpp
#        Experiments/AveBaseline/100K/4.cpp
#        Experiments/AveBaseline/1M/0.cpp
#        Experiments/AveBaseline/1M/1.cpp
#        Experiments/AveBaseline/1M/2.cpp
#        Experiments/AveBaseline/1M/3.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move10.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move100.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move1000.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move10000.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move2.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move20.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move200.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move2000.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move4.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move40.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move400.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move4000.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move6.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move60.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move600.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move6000.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move8.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move80.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move800.cpp
#        Experiments/Baseline_0/100K/2-0706-moveLength/move8000.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap10.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap100.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap1000.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap10000.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap2.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap20.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap200.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap2000.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap4.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap40.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap400.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap4000.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap6.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap60.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap600.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap6000.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap8.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap80.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap800.cpp
#        Experiments/Baseline_0/100K/2-0706-swapLength/swap8000.cpp
#        Experiments/Baseline_0/100K/delete/average.cpp
#        Experiments/Baseline_0/100K/delete/delete.cpp
#        Experiments/Baseline_0/100K/insert/average.cpp
#        Experiments/Baseline_0/100K/insert/insert.cpp
#        Experiments/Baseline_0/100K/move/average.cpp
#        Experiments/Baseline_0/100K/move/move.cpp
#        Experiments/Baseline_0/100K/moveLength/move.cpp
#        Experiments/Baseline_0/100K/moveLength/move10.cpp
#        Experiments/Baseline_0/100K/moveLength/move100.cpp
#        Experiments/Baseline_0/100K/moveLength/move1000.cpp
#        Experiments/Baseline_0/100K/moveLength/move10000.cpp
#        Experiments/Baseline_0/100K/moveLength/move2.cpp
#        Experiments/Baseline_0/100K/moveLength/move20.cpp
#        Experiments/Baseline_0/100K/moveLength/move200.cpp
#        Experiments/Baseline_0/100K/moveLength/move2000.cpp
#        Experiments/Baseline_0/100K/moveLength/move4.cpp
#        Experiments/Baseline_0/100K/moveLength/move40.cpp
#        Experiments/Baseline_0/100K/moveLength/move400.cpp
#        Experiments/Baseline_0/100K/moveLength/move4000.cpp
#        Experiments/Baseline_0/100K/moveLength/move6.cpp
#        Experiments/Baseline_0/100K/moveLength/move60.cpp
#        Experiments/Baseline_0/100K/moveLength/move600.cpp
#        Experiments/Baseline_0/100K/moveLength/move6000.cpp
#        Experiments/Baseline_0/100K/moveLength/move8.cpp
#        Experiments/Baseline_0/100K/moveLength/move80.cpp
#        Experiments/Baseline_0/100K/moveLength/move800.cpp
#        Experiments/Baseline_0/100K/moveLength/move8000.cpp
#        Experiments/Baseline_0/100K/pointquery/pointquery.cpp
#        Experiments/Baseline_0/100K/query/query.cpp
#        Experiments/Baseline_0/100K/queryLength/query.cpp
#        Experiments/Baseline_0/100K/reorder/reorder.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder100.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder1000.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder10000.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder200.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder2000.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder400.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder4000.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder600.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder6000.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder800.cpp
#        Experiments/Baseline_0/100K/reorderLength/reorder8000.cpp
#        Experiments/Baseline_0/100K/swap/average.cpp
#        Experiments/Baseline_0/100K/swap/swap.cpp
#        Experiments/Baseline_0/100K/swapLength/swap.cpp
#        Experiments/Baseline_0/100K/swapLength/swap10.cpp
#        Experiments/Baseline_0/100K/swapLength/swap100.cpp
#        Experiments/Baseline_0/100K/swapLength/swap1000.cpp
#        Experiments/Baseline_0/100K/swapLength/swap10000.cpp
#        Experiments/Baseline_0/100K/swapLength/swap2.cpp
#        Experiments/Baseline_0/100K/swapLength/swap20.cpp
#        Experiments/Baseline_0/100K/swapLength/swap200.cpp
#        Experiments/Baseline_0/100K/swapLength/swap2000.cpp
#        Experiments/Baseline_0/100K/swapLength/swap4.cpp
#        Experiments/Baseline_0/100K/swapLength/swap40.cpp
#        Experiments/Baseline_0/100K/swapLength/swap400.cpp
#        Experiments/Baseline_0/100K/swapLength/swap4000.cpp
#        Experiments/Baseline_0/100K/swapLength/swap6.cpp
#        Experiments/Baseline_0/100K/swapLength/swap60.cpp
#        Experiments/Baseline_0/100K/swapLength/swap600.cpp
#        Experiments/Baseline_0/100K/swapLength/swap6000.cpp
#        Experiments/Baseline_0/100K/swapLength/swap8.cpp
#        Experiments/Baseline_0/100K/swapLength/swap80.cpp
#        Experiments/Baseline_0/100K/swapLength/swap800.cpp
#        Experiments/Baseline_0/100K/swapLength/swap8000.cpp
#        Experiments/Baseline_0/100K/2-0702-pointquery.cpp
#        Experiments/Baseline_0/100K/2-0702-query10.cpp
#        Experiments/Baseline_0/100K/2-0702-query100.cpp
#        Experiments/Baseline_0/100K/2-0702-query1000.cpp
#        Experiments/Baseline_0/100K/2-0702-query50.cpp
#        Experiments/Baseline_0/100K/2-0702-query500.cpp
#        Experiments/Baseline_0/100K/delete100K.cpp
#        Experiments/Baseline_0/100K/insert100K.cpp
#        Experiments/Baseline_0/100K/move100K.cpp
#        Experiments/Baseline_0/100K/pointquery100K.cpp
#        Experiments/Baseline_0/100K/query100K.cpp
#        Experiments/Baseline_0/100K/reorder100K.cpp
#        Experiments/Baseline_0/100K/swap100K.cpp
#        Experiments/Baseline_0/10K/delete/average.cpp
#        Experiments/Baseline_0/10K/delete/delete.cpp
#        Experiments/Baseline_0/10K/insert/average.cpp
#        Experiments/Baseline_0/10K/insert/insert.cpp
#        Experiments/Baseline_0/10K/pointquery/pointquery.cpp
#        Experiments/Baseline_0/10K/query/query.cpp
#        Experiments/Baseline_0/10K/reorder/reorder.cpp
#        Experiments/Baseline_0/10K/swap/average.cpp
#        Experiments/Baseline_0/10K/swap/swap.cpp
#        Experiments/Baseline_0/1M/delete/average.cpp
#        Experiments/Baseline_0/1M/delete/delete.cpp
#        Experiments/Baseline_0/1M/delete/delete1.cpp
#        Experiments/Baseline_0/1M/delete/delete10.cpp
#        Experiments/Baseline_0/1M/delete/delete2.cpp
#        Experiments/Baseline_0/1M/delete/delete3.cpp
#        Experiments/Baseline_0/1M/delete/delete4.cpp
#        Experiments/Baseline_0/1M/delete/delete5.cpp
#        Experiments/Baseline_0/1M/delete/delete6.cpp
#        Experiments/Baseline_0/1M/delete/delete7.cpp
#        Experiments/Baseline_0/1M/delete/delete8.cpp
#        Experiments/Baseline_0/1M/delete/delete9.cpp
#        Experiments/Baseline_0/1M/insert/average.cpp
#        Experiments/Baseline_0/1M/insert/insert.cpp
#        Experiments/Baseline_0/1M/insert/insert1.cpp
#        Experiments/Baseline_0/1M/insert/insert10.cpp
#        Experiments/Baseline_0/1M/insert/insert2.cpp
#        Experiments/Baseline_0/1M/insert/insert3.cpp
#        Experiments/Baseline_0/1M/insert/insert4.cpp
#        Experiments/Baseline_0/1M/insert/insert5.cpp
#        Experiments/Baseline_0/1M/insert/insert6.cpp
#        Experiments/Baseline_0/1M/insert/insert7.cpp
#        Experiments/Baseline_0/1M/insert/insert8.cpp
#        Experiments/Baseline_0/1M/insert/insert9.cpp
#        Experiments/Baseline_0/1M/pointquery/pointquery.cpp
#        Experiments/Baseline_0/1M/query/query.cpp
#        Experiments/Baseline_0/1M/queryLength/query10.cpp
#        Experiments/Baseline_0/1M/queryLength/query100.cpp
#        Experiments/Baseline_0/1M/queryLength/query1000.cpp
#        Experiments/Baseline_0/1M/queryLength/query10000.cpp
#        Experiments/Baseline_0/1M/queryLength/query100000.cpp
#        Experiments/Baseline_0/1M/queryLength/query100Avg.cpp
#        Experiments/Baseline_0/1M/queryLength/query10Avg.cpp
#        Experiments/Baseline_0/1M/queryLength/query5.cpp
#        Experiments/Baseline_0/1M/queryLength/query50.cpp
#        Experiments/Baseline_0/1M/queryLength/query500.cpp
#        Experiments/Baseline_0/1M/queryLength/query5000.cpp
#        Experiments/Baseline_0/1M/queryLength/query50000.cpp
#        Experiments/Baseline_0/1M/queryLength/query50Avg.cpp
#        Experiments/Baseline_0/1M/queryLength/query5Avg.cpp
#        Experiments/Baseline_0/1M/reorder/average.cpp
#        Experiments/Baseline_0/1M/reorder/reorder.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder10.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder100.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder1000.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder10000.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder100000.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder1000Avg.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder100Avg.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder10Avg.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder5.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder50.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder500.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder5000.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder50000.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder500Avg.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder50Avg.cpp
#        Experiments/Baseline_0/1M/reorderLength/reorder5Avg.cpp
#        Experiments/Baseline_0/1M/swap/swap.cpp
#        Experiments/Baseline_0/1M/swapLength/swap10.cpp
#        Experiments/Baseline_0/1M/swapLength/swap100.cpp
#        Experiments/Baseline_0/1M/swapLength/swap1000.cpp
#        Experiments/Baseline_0/1M/swapLength/swap10000.cpp
#        Experiments/Baseline_0/1M/swapLength/swap100000.cpp
#        Experiments/Baseline_0/1M/swapLength/swap5.cpp
#        Experiments/Baseline_0/1M/swapLength/swap50.cpp
#        Experiments/Baseline_0/1M/swapLength/swap500.cpp
#        Experiments/Baseline_0/1M/swapLength/swap5000.cpp
#        Experiments/Baseline_0/1M/swapLength/swap50000.cpp
#        Experiments/Baseline_0/1M/2-0702-query10.cpp
#        Experiments/Baseline_0/1M/2-0702-query100.cpp
#        Experiments/Baseline_0/1M/2-0702-query1000.cpp
#        Experiments/Baseline_0/1M/2-0702-query500.cpp
#        Experiments/Baseline_0/1M/move.cpp
#        Experiments/Baseline_0/1M/query.cpp
#        Experiments/Baseline_0/1M/reorder.cpp
#        Experiments/Baseline_0/1M/swap.cpp
#        Experiments/Baseline_0/1MCBT/delete/average.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete1.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete10.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete2.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete3.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete4.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete5.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete6.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete7.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete8.cpp
#        Experiments/Baseline_0/1MCBT/delete/delete9.cpp
#        Experiments/Baseline_0/1MCBT/insert/average.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert1.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert10.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert2.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert3.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert4.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert5.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert6.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert7.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert8.cpp
#        Experiments/Baseline_0/1MCBT/insert/insert9.cpp
#        Experiments/Baseline_0/1MCBT/pointquery/pointquery.cpp
#        Experiments/Baseline_0/1MCBT/query/query.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query10.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query100.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query1000.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query10000.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query100000.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query100Avg.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query10Avg.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query5.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query50.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query500.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query5000.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query50000.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query50Avg.cpp
#        Experiments/Baseline_0/1MCBT/queryLength/query5Avg.cpp
#        Experiments/Baseline_0/1MCBT/reorder/average.cpp
#        Experiments/Baseline_0/1MCBT/reorder/reorder.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder10.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder100.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder1000.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder10000.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder100000.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder1000Avg.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder100Avg.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder10Avg.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder5.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder50.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder500.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder5000.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder50000.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder500Avg.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder50Avg.cpp
#        Experiments/Baseline_0/1MCBT/reorderLength/reorder5Avg.cpp
#        Experiments/Baseline_0/1MCBT/swap/swap.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap10.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap100.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap1000.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap10000.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap100000.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap5.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap50.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap500.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap5000.cpp
#        Experiments/Baseline_0/1MCBT/swapLength/swap50000.cpp
#        Experiments/Baseline_1_20220109/100K/delete/average.cpp
#        Experiments/Baseline_1_20220109/100K/delete/delete.cpp
#        Experiments/Baseline_1_20220109/100K/insert/average.cpp
#        Experiments/Baseline_1_20220109/100K/insert/insert.cpp
#        Experiments/Baseline_1_20220109/100K/pointquery/average.cpp
#        Experiments/Baseline_1_20220109/100K/pointquery/pointquery.cpp
#        Experiments/Baseline_1_20220109/100K/pointquery/pointquery_original.cpp
#        Experiments/Baseline_1_20220109/100K/query/average.cpp
#        Experiments/Baseline_1_20220109/100K/query/query.cpp
#        Experiments/Baseline_1_20220109/100K/query/query_original.cpp
#        Experiments/Baseline_1_20220109/100K/reorder/average.cpp
#        Experiments/Baseline_1_20220109/100K/reorder/reorder.cpp
#        Experiments/Baseline_1_20220109/100K/swap/average.cpp
#        Experiments/Baseline_1_20220109/100K/swap/swap.cpp
#        Experiments/Baseline_1_20220109/10K/delete/average.cpp
#        Experiments/Baseline_1_20220109/10K/delete/delete.cpp
#        Experiments/Baseline_1_20220109/10K/insert/average.cpp
#        Experiments/Baseline_1_20220109/10K/insert/insert.cpp
#        Experiments/Baseline_1_20220109/10K/pointquery/average.cpp
#        Experiments/Baseline_1_20220109/10K/pointquery/pointquery.cpp
#        Experiments/Baseline_1_20220109/10K/pointquery/pointquery_original.cpp
#        Experiments/Baseline_1_20220109/10K/query/average.cpp
#        Experiments/Baseline_1_20220109/10K/query/query.cpp
#        Experiments/Baseline_1_20220109/10K/query/query2.cpp
#        Experiments/Baseline_1_20220109/10K/query/query_original.cpp
#        Experiments/Baseline_1_20220109/10K/query_repeat_program/average.cpp
#        Experiments/Baseline_1_20220109/10K/query_repeat_program/query.cpp
#        Experiments/Baseline_1_20220109/10K/reorder/average.cpp
#        Experiments/Baseline_1_20220109/10K/reorder/reorder.cpp
#        Experiments/Baseline_1_20220109/10K/swap/average.cpp
#        Experiments/Baseline_1_20220109/10K/swap/swap.cpp
#        Experiments/Baseline_1_20220109/1M/delete/delete.cpp
#        Experiments/Baseline_1_20220109/1M/insert/insert.cpp
#        Experiments/Baseline_1_20220109/1M/pointquery/pointquery.cpp
#        Experiments/Baseline_1_20220109/1M/query/query.cpp
#        Experiments/Baseline_1_20220109/1M/reorder/reorder.cpp
#        Experiments/Baseline_1_20220109/1M/swap/swap.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/delete/average.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/delete/delete.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/insert/average.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/insert/insert.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/pointquery/average.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/pointquery/pointquery.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/query/average.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/query/query.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/reorder/average.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/reorder/reorder.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/swap/average.cpp
#        Experiments/Baseline_1_20220109/1Mrepeat5/swap/swap.cpp
#        Experiments/BasicOperation/CBTDebug/swap1.cpp
#        Experiments/BasicOperation/CBTDebug/swap2.cpp
#        Experiments/BasicOperation/DAonedp/insert.cpp
#        Experiments/BasicOperation/DAonedp/move.cpp
#        Experiments/BasicOperation/DAonedp/randomOp.cpp
#        Experiments/BasicOperation/DAonedp/swap.cpp
#        Experiments/BasicOperation/vector/stackoverflow.cpp
#        Experiments/BasicOperation/vector/vector.cpp
#        Experiments/BasicOperation/vector/vector_vs_array.cpp
#        Experiments/BasicOperation/vector/vector_vs_array_2.cpp
#        Experiments/BasicOperation/insert.cpp
#        Experiments/BasicOperation/insertInAll.cpp
#        Experiments/BasicOperation/move.cpp
#        Experiments/BasicOperation/random.cpp
#        Experiments/BasicOperation/swap.cpp
#        Experiments/BasicOperation/swapDA.cpp
#        Experiments/BasicOperation/swapLL.cpp
#        Experiments/CaseStudy/WhyTVFast/tv_ada.cpp
#        Experiments/CaseStudy/workload/in_paper20210919/message2.cpp
#        Experiments/CaseStudy/workload/message/message1.cpp
#        Experiments/CaseStudy/workload/message/message2.cpp
#        Experiments/CaseStudy/workload/pooling/pool.h
#        Experiments/CaseStudy/workload/pooling/pool_eg.cpp
#        Experiments/CaseStudy/workload/pooling/pool_eg2.cpp
#        Experiments/CaseStudy/workload/pooling/pool_eg3.cpp
#        Experiments/CaseStudy/workload/pooling/pool_eg4.cpp
#        Experiments/CaseStudy/workload/pooling/pooling1.cpp
#        Experiments/CaseStudy/workload/pooling/pooling2.cpp
#        Experiments/CaseStudy/workload/pooling/queue.h
#        Experiments/CaseStudy/workload/pooling/semaphore.h
#        Experiments/CaseStudy/workload/pooling/ThreadPool.h
#        Experiments/CaseStudy/workload/pooling/wl2.cpp
#        Experiments/CaseStudy/workload/stdasync/eg1.cpp
#        Experiments/CaseStudy/workload/wl3/wl3.cpp
#        Experiments/CaseStudy/workload/wl4/pool_eg4.cpp
#        Experiments/CaseStudy/workload/wl4/wl4.cpp
#        Experiments/CaseStudy/workload/try1.cpp
#        Experiments/CaseStudy/workload/try2.cpp
#        Experiments/CaseStudy/workload/wl1.cpp
#        Experiments/CaseStudy/workload/wl2.cpp
#        Experiments/CaseStudy_1/workload/message.cpp
#        Experiments/e2e/100K/delete.cpp
#        Experiments/e2e/100K/insert.cpp
#        Experiments/e2e/100K/move.cpp
#        Experiments/e2e/100K/query.cpp
#        Experiments/e2e/100K/reorder.cpp
#        Experiments/e2e/100K/swap.cpp
#        Experiments/e2e/1M/delete.cpp
#        Experiments/e2e/1M/insert.cpp
#        Experiments/e2e/1M/move.cpp
#        Experiments/e2e/1M/query.cpp
#        Experiments/e2e/1M/reorder.cpp
#        Experiments/e2e/1M/swap.cpp
#        Experiments/e2e2/100K/delete.cpp
#        Experiments/e2e2/100K/insert.cpp
#        Experiments/e2e2/100K/move.cpp
#        Experiments/e2e2/100K/query.cpp
#        Experiments/e2e2/100K/reorder.cpp
#        Experiments/e2e2/100K/swap.cpp
#        Experiments/e2e2/onlyUpdate/delete.cpp
#        Experiments/e2e2/onlyUpdate/insert.cpp
#        Experiments/e2e2/onlyUpdate/query.cpp
#        Experiments/Framework/asyn/1M/asyn1M1.cpp
#        Experiments/Framework/asyn/asyn.cpp
#        Experiments/Framework/asyn/asyn2.cpp
#        Experiments/Framework/asyn/asyn2wofsanitize.cpp
#        Experiments/Framework/asyn/asyn3.cpp
#        Experiments/Framework/asyn/asyn4.cpp
#        Experiments/Framework/asyn/asyn5.cpp
#        Experiments/Framework/asyn/asyn6.cpp
#        Experiments/Framework/asyn/asyn7.cpp
#        Experiments/Framework/asyn/asynNoNewConn.cpp
#        Experiments/Framework/asyn/syn.cpp
#        Experiments/Framework/asyn/updateInThread.cpp
#        Experiments/Framework/prefetching/1M/prefetching1M1.cpp
#        Experiments/Framework/prefetching/asyn.cpp
#        Experiments/Framework/prefetching/prefetching.cpp
#        Experiments/Framework/prefetching/prefetching2.cpp
#        Experiments/Framework/prefetching/prefetching3.cpp
#        Experiments/Framework/prefetching/prefetching4.cpp
#        Experiments/Framework/prefetching/prefetching5.cpp
#        Experiments/Framework/100K.cpp
#        Experiments/Framework/2threads.cpp
#        Experiments/Framework/atomicFlag.cpp
#        Experiments/Framework/exampleAtomic.cpp
#        Experiments/Framework/insert.cpp
#        Experiments/LL/swap10.cpp
#        Experiments/LL/swap102.cpp
#        Experiments/LL/swap103.cpp
#        Experiments/LL/swap11.cpp
#        Experiments/LL/swap12.cpp
#        Experiments/LL/swap13.cpp
#        Experiments/LL/swap4.cpp
#        Experiments/LL/swap5.cpp
#        Experiments/LL/swap6.cpp
#        Experiments/LL/swap7.cpp
#        Experiments/LL/swap8.cpp
#        Experiments/LL/swap9.cpp
#        Experiments/LL/swap92.cpp
#        Experiments/LL/swap93.cpp
#        Experiments/LLApr29/100K/swap0.cpp
#        Experiments/LLApr29/100K/swap1.cpp
#        Experiments/LLApr29/100K/swap2.cpp
#        Experiments/LLApr29/100K/swap3.cpp
#        Experiments/LLApr29/100K/swap4.cpp
#        Experiments/MemoryUsage/CBT.cpp
#        Experiments/MemoryUsage/DA.cpp
#        Experiments/MemoryUsage/LL.cpp
#        Experiments/MemoryUsage/SA.cpp
#        Experiments/mysql/eg1.cpp
#        Experiments/mysql/eg2.cpp
#        Experiments/mysql/eg3.cpp
#        Experiments/mysql/eg4.cpp
#        Experiments/other/1.cpp
#        Experiments/other/swap10.cpp
#        Experiments/other/swap5.cpp
#        Experiments/other/swap52.cpp
#        Experiments/other/swap53.cpp
#        Experiments/Scalability/100/delete.cpp
#        Experiments/Scalability/100/insert.cpp
#        Experiments/Scalability/100/move.cpp
#        Experiments/Scalability/100/pointquery.cpp
#        Experiments/Scalability/100/query.cpp
#        Experiments/Scalability/100/reorder.cpp
#        Experiments/Scalability/100/swap.cpp
#        Experiments/Scalability/100K/delete/delete.cpp
#        Experiments/Scalability/100K/insert/insert.cpp
#        Experiments/Scalability/100K/pointquery/pointquery.cpp
#        Experiments/Scalability/100K/pointquery/pointqueryAvg.cpp
#        Experiments/Scalability/100K/query/query500.cpp
#        Experiments/Scalability/100K/reorder/reorder500.cpp
#        Experiments/Scalability/100K/swap/swap.cpp
#        Experiments/Scalability/10K/delete/average.cpp
#        Experiments/Scalability/10K/delete/delete.cpp
#        Experiments/Scalability/10K/insert/average.cpp
#        Experiments/Scalability/10K/insert/insert.cpp
#        Experiments/Scalability/10K/pointquery/pointquery.cpp
#        Experiments/Scalability/10K/pointquery/pointqueryAvg.cpp
#        Experiments/Scalability/10K/query/average.cpp
#        Experiments/Scalability/10K/query/query.cpp
#        Experiments/Scalability/10K/query/query500.cpp
#        Experiments/Scalability/10K/query/query500Avg.cpp
#        Experiments/Scalability/10K/reorder/average.cpp
#        Experiments/Scalability/10K/reorder/reorder.cpp
#        Experiments/Scalability/10K/reorder/reorder500.cpp
#        Experiments/Scalability/10K/swap/average.cpp
#        Experiments/Scalability/10K/swap/swap.cpp
#        Experiments/Scalability/10K/delete.cpp
#        Experiments/Scalability/10K/insert.cpp
#        Experiments/Scalability/10K/move.cpp
#        Experiments/Scalability/10K/pointquery.cpp
#        Experiments/Scalability/10K/query.cpp
#        Experiments/Scalability/10K/reorder.cpp
#        Experiments/Scalability/10K/swap.cpp
#        Experiments/Scalability/1K/delete/average.cpp
#        Experiments/Scalability/1K/delete/delete.cpp
#        Experiments/Scalability/1K/insert/average.cpp
#        Experiments/Scalability/1K/insert/insert.cpp
#        Experiments/Scalability/1K/pointquery/pointquery.cpp
#        Experiments/Scalability/1K/pointquery/pointqueryAvg.cpp
#        Experiments/Scalability/1K/query/query500.cpp
#        Experiments/Scalability/1K/query/query500Avg.cpp
#        Experiments/Scalability/1K/reorder/reorder500.cpp
#        Experiments/Scalability/1K/reorder/reorder500Avg.cpp
#        Experiments/Scalability/1K/swap/average.cpp
#        Experiments/Scalability/1K/swap/swap.cpp
#        Experiments/Scalability/1M/delete/delete.cpp
#        Experiments/Scalability/1M/insert/insert.cpp
#        Experiments/Scalability/1M/pointquery/pointquery.cpp
#        Experiments/Scalability/1M/pointquery/pointqueryAvg.cpp
#        Experiments/Scalability/1M/query/query500.cpp
#        Experiments/Scalability/1M/reorder/reorder500.cpp
#        Experiments/Scalability/1M/swap/swap.cpp
#        Experiments/Scalability/1M/query.cpp
#        Experiments/Scalability/1M/reorder.cpp
#        Experiments/Scalability/delete/delete100K.cpp
#        Experiments/Scalability/delete/delete10K.cpp
#        Experiments/Scalability/delete/delete1K.cpp
#        Experiments/Scalability/delete/delete1M.cpp
#        Experiments/Scalability/insert/insert100K.cpp
#        Experiments/Scalability/insert/insert10K.cpp
#        Experiments/Scalability/insert/insert1K.cpp
#        Experiments/Scalability/insert/insert1M.cpp
#        Experiments/Scalability/pointquery/pointquery100K.cpp
#        Experiments/Scalability/pointquery/pointquery10K.cpp
#        Experiments/Scalability/pointquery/pointquery1K.cpp
#        Experiments/Scalability/pointquery/pointquery1M.cpp
#        Experiments/Scalability/query500/query100K.cpp
#        Experiments/Scalability/query500/query10K.cpp
#        Experiments/Scalability/query500/query1K.cpp
#        Experiments/Scalability/query500/query1M.cpp
#        Experiments/Scalability/reorder500/reorder100K.cpp
#        Experiments/Scalability/reorder500/reorder10K.cpp
#        Experiments/Scalability/reorder500/reorder1K.cpp
#        Experiments/Scalability/reorder500/reorder1M.cpp
#        Experiments/Scalability/swap/swap100K.cpp
#        Experiments/Scalability/swap/swap10K.cpp
#        Experiments/Scalability/swap/swap1K.cpp
#        Experiments/Scalability/swap/swap1M.cpp
#        Experiments/Scalability/swapFixedLength/swap100K.cpp
#        Experiments/Scalability/swapFixedLength/swap10K.cpp
#        Experiments/Scalability/swapFixedLength/swap1K.cpp
#        Experiments/Scalability/swapFixedLength/swap1M.cpp
#        Experiments/Scalability1Mop/delete/delete100K.cpp
#        Experiments/Scalability1Mop/delete/delete10K.cpp
#        Experiments/Scalability1Mop/delete/delete1K.cpp
#        Experiments/Scalability1Mop/delete/delete1M.cpp
#        Experiments/Scalability1Mop/insert/insert100K.cpp
#        Experiments/Scalability1Mop/insert/insert10K.cpp
#        Experiments/Scalability1Mop/insert/insert1K.cpp
#        Experiments/Scalability1Mop/insert/insert1M.cpp
#        Experiments/Scalability1Mop/pointquery/pointquery100K.cpp
#        Experiments/Scalability1Mop/pointquery/pointquery10K.cpp
#        Experiments/Scalability1Mop/pointquery/pointquery1K.cpp
#        Experiments/Scalability1Mop/pointquery/pointquery1M.cpp
#        Experiments/Scalability1Mop/query500/query100K.cpp
#        Experiments/Scalability1Mop/query500/query10K.cpp
#        Experiments/Scalability1Mop/query500/query1K.cpp
#        Experiments/Scalability1Mop/query500/query1M.cpp
#        Experiments/Scalability1Mop/reorder500/reorder100K.cpp
#        Experiments/Scalability1Mop/reorder500/reorder10K.cpp
#        Experiments/Scalability1Mop/reorder500/reorder1K.cpp
#        Experiments/Scalability1Mop/reorder500/reorder1M.cpp
#        Experiments/Scalability1Mop/swap/swap100K.cpp
#        Experiments/Scalability1Mop/swap/swap10K.cpp
#        Experiments/Scalability1Mop/swap/swap1K.cpp
#        Experiments/Scalability1Mop/swap/swap1M.cpp
#        Experiments/Scalability1Mop/swapFixedLength/swap100K.cpp
#        Experiments/Scalability1Mop/swapFixedLength/swap10K.cpp
#        Experiments/Scalability1Mop/swapFixedLength/swap1K.cpp
#        Experiments/Scalability1Mop/swapFixedLength/swap1M.cpp
#        main.cpp)

add_executable(CBTdebug3
        DataStructures/CBT_Xcode.cpp
        DataStructures/CBT_Xcode.h
        DataStructures/DAnolimit.cpp
        DataStructures/DAnolimit.h
        DataStructures/DAstatic.cpp
        DataStructures/DAstatic.h
        DataStructures/DynamicArray.cpp
        DataStructures/DynamicArray.h
        DataStructures/LinkedList.cpp
        DataStructures/LinkedList.h
        DataStructures/StandardArray.cpp
        DataStructures/StandardArray.h
        DataStructures/tiered-vector.h
        Experiments/TroubleShooting/CBTDebug/debug3.cpp
        )


add_executable(CBTdebug4
        DataStructures/CountedBtree.cpp
        DataStructures/CountedBtree.h
        DataStructures/DAnolimit.cpp
        DataStructures/DAnolimit.h
        DataStructures/DAstatic.cpp
        DataStructures/DAstatic.h
        DataStructures/DynamicArray.cpp
        DataStructures/DynamicArray.h
        DataStructures/LinkedList.cpp
        DataStructures/LinkedList.h
        DataStructures/StandardArray.cpp
        DataStructures/StandardArray.h
        DataStructures/tiered-vector.h
        Experiments/TroubleShooting/CBTDebug/debug4.cpp
        )



add_executable(CBTdebug6
        DataStructures/CountedBtree.cpp
        DataStructures/CountedBtree.h
        DataStructures/DAnolimit.cpp
        DataStructures/DAnolimit.h
        DataStructures/DAstatic.cpp
        DataStructures/DAstatic.h
        DataStructures/DynamicArray.cpp
        DataStructures/DynamicArray.h
        DataStructures/LinkedList.cpp
        DataStructures/LinkedList.h
        DataStructures/StandardArray.cpp
        DataStructures/StandardArray.h
        DataStructures/tiered-vector.h
        Experiments/TroubleShooting/CBTDebug/debug6.cpp
        )



add_executable(CBTdebugswap1
        DataStructures/CountedBtree.cpp
        DataStructures/CountedBtree.h
        DataStructures/DAnolimit.cpp
        DataStructures/DAnolimit.h
        DataStructures/DAstatic.cpp
        DataStructures/DAstatic.h
        DataStructures/DynamicArray.cpp
        DataStructures/DynamicArray.h
        DataStructures/LinkedList.cpp
        DataStructures/LinkedList.h
        DataStructures/StandardArray.cpp
        DataStructures/StandardArray.h
        DataStructures/tiered-vector.h
        Experiments/TroubleShooting/CBTDebug/swap1.cpp
        )





add_executable(Baselineinsert
        DataStructures/CountedBtree.cpp
        DataStructures/CountedBtree.h
        DataStructures/DAnolimit.cpp
        DataStructures/DAnolimit.h
        DataStructures/DAstatic.cpp
        DataStructures/DAstatic.h
        DataStructures/DynamicArray.cpp
        DataStructures/DynamicArray.h
        DataStructures/LinkedList.cpp
        DataStructures/LinkedList.h
        DataStructures/StandardArray.cpp
        DataStructures/StandardArray.h
        DataStructures/tiered-vector.h
        Experiments/Baseline_1_20220109/100K/insert/insert.cpp
        )




add_executable(LL1
        DataStructures/CountedBtree.cpp
        DataStructures/CountedBtree.h
        DataStructures/DAnolimit.cpp
        DataStructures/DAnolimit.h
        DataStructures/DAstatic.cpp
        DataStructures/DAstatic.h
        DataStructures/DynamicArray.cpp
        DataStructures/DynamicArray.h
        DataStructures/LinkedList.cpp
        DataStructures/LinkedList.h
        DataStructures/StandardArray.cpp
        DataStructures/StandardArray.h
        DataStructures/tiered-vector.h
        Experiments/TroubleShooting/LLmemory/LL1.cpp
        )



