

# TP2 de GUSCHING Jonathan

`pandoc -s --toc tp2.md --css=./github-pandoc.css -o tp2.html`





## lscpu

```
Architecture :                          x86_64
Mode(s) opératoire(s) des processeurs : 32-bit, 64-bit
Boutisme :                              Little Endian
Address sizes:                          39 bits physical, 48 bits virtual
Processeur(s) :                         4
Liste de processeur(s) en ligne :       0-3
Thread(s) par cœur :                    1
Cœur(s) par socket :                    4
Socket(s) :                             1
Nœud(s) NUMA :                          1
Identifiant constructeur :              GenuineIntel
Famille de processeur :                 6
Modèle :                                94
Nom de modèle :                         Intel(R) Core(TM) i5-6300HQ CPU @ 2.30GH
                                  
Révision :                              3
Vitesse du processeur en MHz :          2300.000
Vitesse maximale du processeur en MHz : 3200,0000
Vitesse minimale du processeur en MHz : 800,0000
BogoMIPS :                              4599.93
Virtualisation :                        VT-x
Cache L1d :                             128 KiB
Cache L1i :                             128 KiB
Cache L2 :                              1 MiB
Cache L3 :                              6 MiB
Nœud NUMA 0 de processeur(s) :          0-3
Vulnerability Itlb multihit:            KVM: Mitigation: VMX disabled
Vulnerability L1tf:                     Mitigation; PTE Inversion; VMX condition
                                        al cache flushes, SMT disabled
Vulnerability Mds:                      Mitigation; Clear CPU buffers; SMT disab
                                        led
Vulnerability Meltdown:                 Mitigation; PTI
Vulnerability Spec store bypass:        Mitigation; Speculative Store Bypass dis
                                        abled via prctl and seccomp
Vulnerability Spectre v1:               Mitigation; usercopy/swapgs barriers and
                                         __user pointer sanitization
Vulnerability Spectre v2:               Mitigation; Full generic retpoline, IBPB
                                         conditional, IBRS_FW, STIBP disabled, R
                                        SB filling
Vulnerability Srbds:                    Mitigation; Microcode
Vulnerability Tsx async abort:          Mitigation; Clear CPU buffers; SMT disab
                                        led
Drapaux :                               fpu vme de pse tsc msr pae mce cx8 apic 
                                        sep mtrr pge mca cmov pat pse36 clflush 
                                        dts acpi mmx fxsr sse sse2 ss ht tm pbe 
                                        syscall nx pdpe1gb rdtscp lm constant_ts
                                        c art arch_perfmon pebs bts rep_good nop
                                        l xtopology nonstop_tsc cpuid aperfmperf
                                         pni pclmulqdq dtes64 monitor ds_cpl vmx
                                         est tm2 ssse3 sdbg fma cx16 xtpr pdcm p
                                        cid sse4_1 sse4_2 x2apic movbe popcnt ts
                                        c_deadline_timer aes xsave avx f16c rdra
                                        nd lahf_lm abm 3dnowprefetch cpuid_fault
                                         epb invpcid_single pti ssbd ibrs ibpb s
                                        tibp tpr_shadow vnmi flexpriority ept vp
                                        id ept_ad fsgsbase tsc_adjust bmi1 hle a
                                        vx2 smep bmi2 erms invpcid rtm mpx rdsee
                                        d adx smap clflushopt intel_pt xsaveopt 
                                        xsavec xgetbv1 xsaves dtherm ida arat pl
                                        n pts hwp hwp_notify hwp_act_window hwp_
                                        epp md_clear flush_l1d
 
```

*Des infos utiles s'y trouvent : nb core, taille de cache*



## Produit scalaire 
On utilise les optimisations fournies par le Makefile habituel (-O3 etc...). On utilise NbSamples assez grand (4112) pour voir clairement la différence d'accélération.

Version OMP:
OMP_NUM    | samples=4112 |
-----------|--------------|
séquentiel | 0.63s        | 
1          | 0.62s        | 
2          | 0.45s        | 
3          | 0.43s        | 
4          | 0.43s        | 
8          | 0.58s        | 

Version C++ 2011
num_threads| samples=4112 |
-----------|--------------| 
1          | 0.83s        | 
2          | 0.61s        | 
3          | 0.62s        | 
4          | 0.58s        | 
8          | 0.75s        | 

On remarque que la version parallèle par OpenMP est plus efficace que l'implémentation standard C++ 2011.

Utiliser 8 threads est plus lent que 4 car on dépasse la limite du processeur (4 threads), ce qui n'est pas optimal. Aussi, on est vite limité par le temps d'accès aux données (u et v). Même avec beaucoup de threads, la tâche est tellement simple (une multiplication assignée à une variable) que l'accès est ce qui prend le plus de temps.



## Produit matrice-matrice



### Permutation des boucles

*Expliquer comment est compilé le code (ligne de make ou de gcc) : on aura besoin de savoir l'optim, les paramètres, etc. Par exemple :*

`make TestProduct.exe && ./TestProduct.exe 1024`


  ordre           | time    | MFlops  | MFlops(n=2048) 
------------------|---------|---------|----------------
i,j,k (origine)   | 8.24619 | 260.421 | 119               
j,i,k             | 7.80256 | 275.228 | 120.241   
i,k,j             | 22.9767 | 96.4635 | Calcul trop long
k,i,j             | 34.4854 | 62.2722 | Calcul trop long   
j,k,i             | 1.1778  | 1823.3  | 1786.88   
k,j,i             | 0.9825  | 2185.62 | 1679.22   


L'accès le plus rapide, est, comme nous l'avons vu en cours, lorsque l'accès mémoire est continu, i.e. k, j, i. Plus il y a de sauts mémoire et plus l'exécution est longue.



### OMP sur la meilleure boucle 

`make TestProduct.exe && OMP_NUM_THREADS=8 ./TestProduct.exe 1024`

  OMP_NUM         | MFlops  | MFlops(n=2048) | MFlops(n=512)  | MFlops(n=4096)
------------------|---------|----------------|----------------|---------------
1                 | 1774.49 | 1650.08        | 1726.02        | 1610
2                 | 2879.6  | 1751.52        | 3524.46        | 1613.89
3                 | 3311.94 | 1590           | 4529.73        | 1596.64
4                 | 3404.56 | 1681           | 5724.15        | 1550.32
5                 | 2136.38 | 1614.45        | 3236.15        | 1577.19
6                 | 2380.26 | 1649.34        | 3304.25        | 1505.91
7                 | 2486.29 | 1650.08        | 3491.9         | 1583.07
8                 | 2380.03 | 1629.48        | 3436.12        | 1556.74




### Produit par blocs

`make TestProduct.exe && ./TestProduct.exe 1024`

  szBlock         | MFlops  | MFlops(n=2048) | MFlops(n=512)  | MFlops(n=4096)
------------------|---------|----------------|----------------|---------------
32                | 2169    | 1780           | 2425           | 1784
64                | 2087    | 2145           | 2230           | 2149
128               | 2462    | 2351           | 2437           | 2373
256               | 2558    | 2252           | 2508           | 1667
512               | 2627    | 2097           | 2660           | 1842
1024              | 2031    | 1850           | 2683           | 1686




### Bloc + OMP



  szBlock      | OMP_NUM | MFlops  | MFlops(n=2048) | MFlops(n=512)  | MFlops(n=4096)
---------------|---------|---------|------------------------------------------------
A.nbCols       |  1      | 1886    | 1700           | 2626           | 1605
A.nbCols       |  4      | 2055    | 1692           | 2653           | 1607  
32             |  4      | 7250    | 6252           | 6789           | 6199
128            |  4      | 7789    | 2995           | 7500           | 1933

Youpi! On arrive à avoir de biens meilleurs résultats!






# Tips 

```
	env 
	OMP_NUM_THREADS=4 ./dot_product.exe
```

```
    $ for i in $(seq 1 4); do elap=$(OMP_NUM_THREADS=$i ./TestProductOmp.exe|grep "Temps CPU"|cut -d " " -f 7); echo -e "$i\t$elap"; done > timers.out
```
