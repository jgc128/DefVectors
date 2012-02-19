@echo off
mkdir results

for %%K IN (1,2,5,10) DO bin\serelex_win32.exe -c "sample-data\concepts.csv" -d "sample-data\definitions.csv" -s "sample-data\stoplist.csv" -o "results\overlap_knn_%%K.csv" -S o -M 1 -K %%K

for %%K IN (1,2,5,10) DO bin\serelex_win32.exe -c "sample-data\concepts.csv" -d "sample-data\definitions.csv" -s "sample-data\stoplist.csv" -o "results\overlap_mutual-knn_%%K.csv" -S o -M 2 -K %%K

for %%K IN (1,2,5,10) DO bin\serelex_win32.exe -c "sample-data\concepts.csv" -d "sample-data\definitions.csv" -s "sample-data\stoplist.csv" -o "results\cos_knn_%%K.csv" -S c -M 1 -K %%K

for %%K IN (1,2,5,10) DO bin\serelex_win32.exe -c "sample-data\concepts.csv" -d "sample-data\definitions.csv" -s "sample-data\stoplist.csv" -o "results\cos_mutual-knn_%%K.csv" -S c -M 2 -K %%K