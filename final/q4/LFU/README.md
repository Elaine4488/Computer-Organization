# Question 4: Modify last level cache policy based on frequency based replacement policy

## 說明

本題目目標為將 GEM5 中 L3 Cache 的替換策略（Replacement Policy）更改為頻率為基礎的 LFU (Least Frequently Used），並使用相同的 benchmark（quicksort）與 Q3 中的 2-way set associative 的預設策略 LRU（Least Recently Used）做比較。

## 實作步驟

1. **修改 Replacement Policy 為 LFU**  
   `configs/common/Caches.py` 中指定 L3 使用 LFURP 作為替換策略。
   ```python
   replacement_policy = Param.BaseReplacementPolicy(LFURP(),"Replacement policy")
   ```
   
3. **編譯 quicksort 執行檔**  
   在 `gem5/benchmark` 執行以下指令編譯成執行檔：  
   ```bash
   gcc --static quicksort.c -o quicksort

4. **混合編譯 GEM5 + NVMain**  
   在 `gem5` 根目錄使用以下指令：
   ```bash
   scons EXTRAS=../NVmain build/X86/gem5.opt

5. **LFURP 策略 模擬測試**  
   將 `--l3_assoc` 設為 2，執行以下指令：
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c benchmark/quicksort --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=2 --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=1MB --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
   ```
 
   > system.l3.overall_miss_rate::total = 0.461621

6. **結果分析**  
   開啟 `m5out/stats.txt`，比較兩種不同策略的 Miss Rate，在 array size 為 `500000` 的情況下:
    - Q3 (LRU): 0.433378
    - Q4 (LFU): 0.461621
