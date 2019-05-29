`py smpTestNS.py  random_graph plan_random group_1 dataset1_1_1_1 1 4 3 0 1200 50`

`py smpTest.py random_graph plan_random group_1 dataset1_1_1_1 536870910 4 1 0 0 999999 1200 0.2 1200 0.2`

`py smpParamAdjust.py  random_graph plan_random group_1 dataset1_1_3_2 1 4 3 0 1200 10`

`SMP_1271_test_ns.exe D:\GitHub\Repo\SMP\test\data\random_graph\plan_random\group_1\dataset1_1_1_1\animal_1.txt 4 1 0 0 3600 1200 0.2 1200 0.2`

`./bin/elpp -f /home/mark/Documents/cplexProj/ELPP-IP-master/data/toy.dat -s 1 -t 5 -c 2`

- 25 1s: 33554431
- 26th 1: 33554432
- 27th 1: 67108864

|graph|OPT|
| --- | --- |
|1_1_1_2  1 | (11252)|
|1_1_3_2 256 |(22760)|
|1_1_1_1 1 |(10530)|
|1_1_1_1 16 |(10133)|