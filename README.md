# 3-uzduotis

## Eksperimentai su sukurtąja Vector klasės realizacija:

### 1 dalis:
#### std::vector pagrindinis pavyzdys:
![Vector main example](Vector%20main%20example.png)
![Vector main example result](Vector%20main%20result.png)
#### std::vector.eempty() pavyzdys:
![Vector empty example](Vector%20empty%20example.png)
![Vector empty result](Vector%20empty%20result.png)
#### std::vector.pop_back() pavyzdys:
![Vector pop_back example](Vector%20pop_back%20example.png)
![Vector pop_back result](Vector%20pop_back%20result.png)
#### std::vector.size() pavyzdys:
![Vector size example](Vector%20size%20example.png)
![Vector size result](Vector%20size%20result.png)
#### std::vector.swap() pavyzdys:
![Vector swap example](Vector%20swap%20example.png)
![Vector swap result](Vector%20swap%20result.png)
### 2 dalis:

#### std::vector ir Vector užpildymo spartos palyginimas(push_back()):
Duomenų kiekis | std::vector | Vector
-------------- | ----------- | ------
10000 | 0.0000685s | 0.0000601s
100000 |  0.0008158s | 0.0005407s
1000000 | 0.0084124s | 0.0046075s
10000000 | 0.0947367s | 0.0676143s
100000000 | 1.01252s | 0.549155s

### 3 dalis:

#### std::vector ir Vector perskirstymų skaičiaus užpildant vektorių 100000000 studentų palyginimas:

Naudota realizacija | Perskirstymai
------------------- | -------------
std::vector | 45
Vector | 

Programa su Vector neveikia...

### 4 dalis:

#### std::vector ir Vector realizacijų spartos palyginimas su 100000 studentų:
Realizacija | Studentų generavimas | Failų generavimas | Studentų rikiavimas | Studentų dalijimas | Pažangių studentų surašymas į failą | Nepažangių studentų surašymas į failą | Visas programos veikimo laikas
----------- | -------------------- | ----------------- | ------------------- | ------------------ | ------------------------------------------- | ------------------------------------- | ------------------------------
std::vector | 1.95959s | 0.507671s | 0.242042s | 0.0378362s | 0.350116s | 0.359497s | 3.48306s
Vector | 

Programa su Vector neveikia...

