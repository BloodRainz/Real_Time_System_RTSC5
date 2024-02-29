##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=RTS-Lab
ConfigurationName      :=Debug
WorkspacePath          :=D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3
ProjectPath            :=D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Kavineshver
Date                   :=29/02/2024
CodeLitePath           :=C:/cseapp/CodeLite
LinkerName             :=$(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
SharedObjectLinkerName :=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).elf
Preprocessors          :=$(PreprocessorSwitch)STM32F40_41xxx 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RTS-Lab.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=
LinkOptions            :=  --specs=nano.specs -mthumb -o $(IntermediateDirectory)/$(ProjectName).elf  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -nostartfiles -T$(ProjectPath)/md407-ram.x -Wl,-Map=$(IntermediateDirectory)/$(ProjectName).map,--cref
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)$(ProjectPath) $(IncludeSwitch)$(ProjectPath)/device/inc $(IncludeSwitch)$(ProjectPath)/driver/inc 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-ar.exe rcu
CXX      := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
CC       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fverbose-asm $(Preprocessors)
ASFLAGS  := 
AS       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\cseapp\CodeLite
Objects0=$(IntermediateDirectory)/musicPlayer.c$(ObjectSuffix) $(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/toneGenerator.c$(ObjectSuffix) $(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix) $(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix) $(IntermediateDirectory)/application.c$(ObjectSuffix) $(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/dispatch.s$(ObjectSuffix) \
	$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	arm-none-eabi-objcopy -S -O srec  ./Debug/RTS-Lab.elf ./Debug/RTS-Lab.s19
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/musicPlayer.c$(ObjectSuffix): musicPlayer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/musicPlayer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/musicPlayer.c$(DependSuffix) -MM musicPlayer.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/musicPlayer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/musicPlayer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/musicPlayer.c$(PreprocessSuffix): musicPlayer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/musicPlayer.c$(PreprocessSuffix) musicPlayer.c

$(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix): sciTinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sciTinyTimber.c$(DependSuffix) -MM sciTinyTimber.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/sciTinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sciTinyTimber.c$(PreprocessSuffix): sciTinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sciTinyTimber.c$(PreprocessSuffix) sciTinyTimber.c

$(IntermediateDirectory)/toneGenerator.c$(ObjectSuffix): toneGenerator.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/toneGenerator.c$(ObjectSuffix) -MF$(IntermediateDirectory)/toneGenerator.c$(DependSuffix) -MM toneGenerator.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/toneGenerator.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/toneGenerator.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/toneGenerator.c$(PreprocessSuffix): toneGenerator.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/toneGenerator.c$(PreprocessSuffix) toneGenerator.c

$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c

$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix): driver/src/stm32f4xx_dac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(DependSuffix) -MM driver/src/stm32f4xx_dac.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_dac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(PreprocessSuffix): driver/src/stm32f4xx_dac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(PreprocessSuffix) driver/src/stm32f4xx_dac.c

$(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix): TinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/TinyTimber.c$(DependSuffix) -MM TinyTimber.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/TinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TinyTimber.c$(PreprocessSuffix): TinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TinyTimber.c$(PreprocessSuffix) TinyTimber.c

$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix): driver/src/stm32f4xx_tim.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(DependSuffix) -MM driver/src/stm32f4xx_tim.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_tim.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(PreprocessSuffix): driver/src/stm32f4xx_tim.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(PreprocessSuffix) driver/src/stm32f4xx_tim.c

$(IntermediateDirectory)/application.c$(ObjectSuffix): application.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/application.c$(ObjectSuffix) -MF$(IntermediateDirectory)/application.c$(DependSuffix) -MM application.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/application.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/application.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/application.c$(PreprocessSuffix): application.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/application.c$(PreprocessSuffix) application.c

$(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix): canTinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/canTinyTimber.c$(DependSuffix) -MM canTinyTimber.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/canTinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/canTinyTimber.c$(PreprocessSuffix): canTinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/canTinyTimber.c$(PreprocessSuffix) canTinyTimber.c

$(IntermediateDirectory)/dispatch.s$(ObjectSuffix): dispatch.s
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dispatch.s$(ObjectSuffix) -MF$(IntermediateDirectory)/dispatch.s$(DependSuffix) -MM dispatch.s
	$(AS) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/dispatch.s" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dispatch.s$(ObjectSuffix) -I$(IncludePath)
$(IntermediateDirectory)/dispatch.s$(PreprocessSuffix): dispatch.s
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dispatch.s$(PreprocessSuffix) dispatch.s

$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix): driver/src/stm32f4xx_gpio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(DependSuffix) -MM driver/src/stm32f4xx_gpio.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_gpio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(PreprocessSuffix): driver/src/stm32f4xx_gpio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(PreprocessSuffix) driver/src/stm32f4xx_gpio.c

$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix): driver/src/stm32f4xx_rcc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(DependSuffix) -MM driver/src/stm32f4xx_rcc.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_rcc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(PreprocessSuffix): driver/src/stm32f4xx_rcc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(PreprocessSuffix) driver/src/stm32f4xx_rcc.c

$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix): driver/src/stm32f4xx_usart.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(DependSuffix) -MM driver/src/stm32f4xx_usart.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_usart.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(PreprocessSuffix): driver/src/stm32f4xx_usart.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(PreprocessSuffix) driver/src/stm32f4xx_usart.c

$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix): driver/src/stm32f4xx_can.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(DependSuffix) -MM driver/src/stm32f4xx_can.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_can.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(PreprocessSuffix): driver/src/stm32f4xx_can.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(PreprocessSuffix) driver/src/stm32f4xx_can.c

$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix): driver/src/stm32f4xx_exti.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(DependSuffix) -MM driver/src/stm32f4xx_exti.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_exti.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(PreprocessSuffix): driver/src/stm32f4xx_exti.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(PreprocessSuffix) driver/src/stm32f4xx_exti.c

$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix): driver/src/stm32f4xx_syscfg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(DependSuffix) -MM driver/src/stm32f4xx_syscfg.c
	$(CC) $(SourceSwitch) "D:/Embedded_Electronics_(MPEESD)/Real_Time_Systems/Codelite/Part2_v3/RTS-Lab/driver/src/stm32f4xx_syscfg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(PreprocessSuffix): driver/src/stm32f4xx_syscfg.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(PreprocessSuffix) driver/src/stm32f4xx_syscfg.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


