# Final Project - Question 3: Configure L3 Cache Associativity and Test Performance

## 說明

本題目目標為設定 GEM5 中 L3 Cache 的組合度（2-way 與 Fully-associative），並使用 quicksort 程式進行性能測試與比較。

## 實作步驟

1. **準備 quicksort 程式**  
   將助教提供的 `quicksort.c` 放入 GEM5 根目錄，並將其中的 array size 修改為 `500000`。

2. **編譯 quicksort 執行檔**  
   在 GEM5 根目錄下開啟 terminal，執行以下指令編譯成執行檔：  
   ```bash
   gcc --static quicksort.c -o quicksort

3. **混合編譯 GEM5 + NVMain**	
   在 gem5 根目錄使用以下指令：
   ```bash
   scons EXTRAS=../NVmain build/X86/gem5.opt

4. **2-way Associative 模擬測試**	
   將 --l3_assoc 設為 2，執行以下指令：
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c quicksort --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=2 --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=1MB --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
 
 > system.l3.overall_miss_rate::total = 0.433378

5. **Fully-way Associative 模擬測試**	
   將 --l3_assoc 設為 1，執行以下指令：
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c quicksort --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=1 --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=1MB --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
 
   <pre> <code> system.l3.overall_miss_rate::total = 0.396614 </code> <pre>

6. **結果分析**	
   開啟 m5out/stats.txt，比較兩種組合度設定下的 Miss Rate，在 array size 擴增為 500000 的情況下:
    - 2-way associative 的 Miss Rate 較高
    - fully associative 的 Miss Rate 較低





