class Platform:
    def getGUI(self):
        raise Exception('Unimplemented')

    def getCOMID(self):
        raise Exception('Unimplemented')

    def getCompilerDefs(self, soenv):
        raise Exception('Unimplemented')

    def getCPUDefs(self, soenv):
        raise Exception('Unimplemented')

    def getOSDefs(self, soenv):
        raise Exception('Unimplemented')

    def getLibraryDefs(self, soenv):
        raise Exception('Unimplemented')

    def getCFlags(self, soenv):
        raise Exception('Unimplemented')

    def getCXXFlags(self, soenv):
        raise Exception('Unimplemented')

    def getInclude(self, soenv):
        raise Exception('Unimplemented')

    def getIncludeStl(self, soenv):
        raise Exception('Unimplemented')

    def getExceptionFlags(self, soenv, enabled):
        raise Exception('Unimplemented')

    def getCompilerOptFlags(self, enabled, debugLevel):
        raise Exception('Unimplemented')

    def getDebugCFlags(self, compiler, enableSymbols):
        raise Exception('Unimplemented')

    def getLibraryLDFlags(self, soenv, layer, outDirLocation, debugging, debugLevel):
        raise Exception('Unimplemented')

    def getStandardLibs(self):
        raise Exception('Unimplemented')