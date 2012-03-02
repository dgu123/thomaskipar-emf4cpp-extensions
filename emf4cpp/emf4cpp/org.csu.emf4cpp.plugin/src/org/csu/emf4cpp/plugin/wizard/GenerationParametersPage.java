package org.csu.emf4cpp.plugin.wizard;

import org.eclipse.jface.layout.GridDataFactory;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.*;

public class GenerationParametersPage extends WizardPage {

    private Text ecPath = null;
    private Text targetDir = null; 
    private Button buttonGenCMakeFiles = null;
    private Button buttonUseGc = null;
    
    protected GenerationParametersPage(String pageName) {
        super(pageName);

        setTitle("EMF4CPP C++ code generator");
        setDescription("Configure generation parameters");
    }
    
    public String getECPath()
    {
        if (ecPath == null)
            return "";
        return ecPath.getText();
    }
    
    public String getTargeDir()
    {
        if (targetDir == null)
            return "";
        return targetDir.getText();
    }
    
    public boolean getGenerateCMakeFiles()
    {
        if (buttonGenCMakeFiles == null)
            return true;
        return buttonGenCMakeFiles.getSelection();
    }

    public boolean getUseGc()
    {
        if (buttonUseGc == null)
            return true;
        return buttonUseGc.getSelection();
    }
    
    @Override
    public void createControl(Composite parent) {

        final Shell shell = getShell();
        
        Composite composite = new Composite(parent, SWT.NONE);
        GridLayout layout = new GridLayout();
        layout.numColumns = 3;
        composite.setLayout(layout);
        setControl(composite);
        
        // EMF4CPP libraries path
        
        new Label(composite, SWT.NONE).setText("EMF4CPP libraries path");

        ecPath = new Text(composite, SWT.BORDER);
        GridData data = new GridData(GridData.FILL_HORIZONTAL);
        ecPath.setLayoutData(data);

        // Clicking the button will allow the user
        // to select a directory
        Button button = new Button(composite, SWT.PUSH);
        button.setText("Browse...");
        button.addSelectionListener(new SelectionAdapter() {
          public void widgetSelected(SelectionEvent event) {
            DirectoryDialog dlg = new DirectoryDialog(shell);

            dlg.setFilterPath(ecPath.getText());
            dlg.setText("EMF4CPP libraries path");
            dlg.setMessage("Select a directory");

            String dir = dlg.open();
            if (dir != null) {
              ecPath.setText(dir);
            }
          }
        });
        
        // Output directory
        
        new Label(composite, SWT.NONE).setText("Generation output directory");

        targetDir = new Text(composite, SWT.BORDER);
        targetDir.setLayoutData(data);

        // Clicking the button will allow the user
        // to select a directory
        button = new Button(composite, SWT.PUSH);
        button.setText("Browse...");
        button.addSelectionListener(new SelectionAdapter() {
          public void widgetSelected(SelectionEvent event) {
            DirectoryDialog dlg = new DirectoryDialog(shell);

            dlg.setFilterPath(ecPath.getText());
            dlg.setText("Generation output directory");
            dlg.setMessage("Select a directory");

            String dir = dlg.open();
            if (dir != null) {
                targetDir.setText(dir);
            }
          }
        });
        
        // cmake files
        
        new Label(composite, SWT.NONE).setText("Generate CMake Files");
        buttonGenCMakeFiles = new Button(composite, SWT.CHECK);
        buttonGenCMakeFiles.setLayoutData(GridDataFactory.swtDefaults().create());
        new Label(composite, SWT.NONE).setText(""); // dummy
        
        // use gc
        
        new Label(composite, SWT.NONE).setText("Use GC");
        buttonUseGc = new Button(composite, SWT.CHECK);
        buttonUseGc.setLayoutData(GridDataFactory.swtDefaults().create());
        buttonUseGc.setSelection(true);
        new Label(composite, SWT.NONE).setText(""); // dummy
    }

}
