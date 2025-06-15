# Question 5: Test the performance of write back and write through policy based on 4-way associative cache with isscc_pcm

## 說明

本題目目標為將 GEM5 中 L3 採用 4-way associative 下，使用 **Write Back**（預設）與 **Write Through** 兩種寫入策略的效能差異。透過 benchmark `multiply` 分析兩種寫入策略下的記憶體寫入請求數（Write Requests) 。

## 實作步驟

1. **修改 Replacement Policy 回 LRU**  
   移除 `configs/common/Caches.py` 中 L3 替換策略 LFURP 。
   ```python
   replacement_policy = Param.BaseReplacementPolicy(LFURP(),"Replacement policy")
   ```
   
2. **編譯 multiply 執行檔**  
   在 `gem5/benchmark` 執行以下指令編譯成執行檔：  
   ```bash
   gcc --static multiply.c -o multiply

3. **混合編譯 GEM5 + NVMain**  
   在 `gem5` 根目錄使用以下指令：
   ```bash
   scons EXTRAS=../NVmain build/X86/gem5.opt

4. **WB 模擬測試**  
   將 `--l3_assoc` 設為 4，執行以下指令：
   ```bash
   ./build/X86/gem5.opt configs/example/se.py -c benchmark/multiply --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=4 --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=1MB --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > terminal_output.txt
   ```
 
   > system.mem_ctrls.num_writes::total = 694

5. **WT 模擬測試**   
   `src/mem/cache/base.cc` 中在 write hit 時，主動發送一個 WriteClean 封包寫入主記憶體，模擬 write-through 策略下每次寫入都同步更新記憶體的行為:
   ```python
   if (blk->isWritable()){
	PacketPtr writeclean_pkt = writecleanBlk(blk, pkt->req->getDest(), pkt->id);
	writebacks.push_back(writeclean_pkt);
   }
   ```
   重複執行指令3-4: 
   > system.mem_ctrls.num_writes::total = 13906264

6. **結果分析**  
   開啟 `m5out/stats.txt`，比較兩種不同策略的 totalWriteRequests :
    - WB: 694
    - WT: 7153759
