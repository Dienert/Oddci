Oddci
=====

The  On-Demand Distributed Computing Infrastructure  (OddCI)  makes use  of  a  broadcast communication network for allocating a large-scale set of processors aimed at High Throughput Computing  (HTC).  A  broadcast network  example is  a  Digital TV system whose  signal  is  transmitted  to  thousands  of  receivers  simultaneously.  These  receivers are  machines  with  significant  processing  power  available  in  our  houses  in  increasing quantities  and  can be used as processing units. However, these potential  processors  are not completely dedicated,  should  be  voluntarily ceded  and may fail (turned  off during the use), which makes them highly volatile resources. In other words, there are no guarantees about the time they  remain dedicated to a task. Thus, it is necessary to use mechanisms able to  deal with this volatility and  to  optimize the collective availability  of these  devices. This  work  aims at  investigating  OddCI  architecture  coordination heuristics, seeking  for  intelligent  ways  to  allocate  or  release  devices  under  the  coverage  of  the broadcast  network  through  sending  collective  messages  with  the  goal  of  coordinating the  allocated  processors  amount.  In order to meet  the  established  Service-Level Agreement (SLA),  factors as resources  population, volatility and  number of  simultaneous requests  are considered  among  others.  The  efficiency  of  the  coordination  heuristics  has been studied  in a  Digital TV network  environment, through experiments  with  simulation. As results, we identified the most significant factors, the resulting effects and restrictions  in  the  evaluated  scenarios.  In  the  scenario  where  the  backend  infrastructure has limited capacity, the main factors were the size of the application image used by the instances  and  the  number  of  concurrent  instances,  meeting  the  most  extreme  case  of 4MB  applications,  80 concurrent instances  and volatilities of 40% with  50% of  the  required parallelism. In the scenario where the minimum makespan was the goal, the main factors were the volatility, the population  (and the devices  availability), and  meeting 50 concurrent  instances  with  reduction  of  only  15%  of  the  required  average  flow  in  the case of  the smaller population with  higher volatilities of up to 40%, showing how far the results have been favorable in each scenario.

Key-words: grid computing, high throughput computing, distributed systems.
