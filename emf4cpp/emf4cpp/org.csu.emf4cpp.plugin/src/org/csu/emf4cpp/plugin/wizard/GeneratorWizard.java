package org.csu.emf4cpp.plugin.wizard;

import org.eclipse.jface.wizard.Wizard;

public class GeneratorWizard extends Wizard {

    GenerationParametersPage genParamsPage;

    public void addPages() {
        genParamsPage = new GenerationParametersPage(
                "EMF4CPP Generation Pararametes Page");
        addPage(genParamsPage);
    }

    private String ecPath = null;
    private String targetDir = null;
    private boolean generateCMakeFiles = true;
    
    public String getECPath()
    {
        return ecPath;
    }
    
    public String getTargeDir()
    {
        return targetDir;
    }
    
    public boolean getGenerateCMakeFiles() 
    {
    	return generateCMakeFiles;
    }
    
    @Override
    public boolean performFinish() {
        
        ecPath = genParamsPage.getECPath();
        targetDir = genParamsPage.getTargeDir();
        generateCMakeFiles = genParamsPage.getGenerateCMakeFiles();
        
        return true;
    }

}
