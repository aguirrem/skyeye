# -*- coding: UTF-8 -*-
import platform


os_info = platform.system()
if cmp(os_info, "Linux"):
        FontMainTitle = "ÌìÄ¿·ÂÕæÆœÌš"
        FontInfoRegs = "²é¿ŽŒÄŽæÆ÷"
        FontShowMems = "²é¿ŽÄÚŽæ"
        FontRemoteGdb = "Ô¶³Ìµ÷ÊÔ"
        FontOpenConfig = "Žò¿ªÅäÖÃÎÄŒþ"
        FontOpenSnapshot = "Žò¿ª¿ìÕÕÎÄŒþ"
        FontSaveSnapshot = "±£Žæ¿ìÕÕÎÄŒþ"
        FontQuit = "ÍË³ö"
        FontUserManu = "ÓÃ»§ÊÖ²á"
        FontVersion = "°æ±ŸÐÅÏ¢"
        FontFileMenu = "ÎÄŒþ"
        FontDebugMenu = "µ÷ÊÔ"
        FontHelpMenu = "°ïÖú"
        FontCPU = "ŽŠÀíÆ÷"
        FontStatus = "×ŽÌ¬"
        FontRun = "ÔËÐÐ"
        FontStop = "Í£Ö¹"
        FontSet = "ÉèÖÃ"
        FontReg = "ŒÄŽæÆ÷"
        FontHex = "Ê®ÁùœøÖÆ"
        FontDec = "Ê®œøÖÆ"
        FontMems = "ÄÚŽæ"
        FontAddr = "µØÖ·"
        FontOK = "È·¶š"
        FontUp = "ÏòÉÏ"
        FontDown = "ÏòÏÂ"
else:
        FontMainTitle = "天目仿真平台"
        FontInfoRegs = "查看寄存器"
        FontShowMems = "查看内存"
        FontRemoteGdb = "远程调试"
        FontOpenConfig = "打开配置文件"
        FontOpenSnapshot = "打开快照文件"
        FontSaveSnapshot = "保存快照文件"
        FontQuit = "退出"
        FontUserManu = "用户手册"
        FontVersion = "版本信息"
        FontFileMenu = "文件"
        FontDebugMenu = "调试"
        FontHelpMenu = "帮助"
        FontCPU = "处理器"
        FontStatus = "状态"
        FontRun = "运行"
        FontStop = "停止"
        FontSet = "设置"
        FontReg = "寄存器"
        FontHex = "十六进制"
        FontDec = "十进制"
        FontMems = "内存"
        FontAddr = "地址"
        FontOK = "确定"
        FontUp = "向上"
        FontDown = "向下"