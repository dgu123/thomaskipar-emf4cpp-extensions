package org.csu.template;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EOperation;


/**
 * Utility class to create the using statements.
 * 
 * @author Thomas Kipar
 */
public class UsingHelper {

	/**
	 * For a given class, this method will return all operation names. Each name will
	 * only be included once in the result, different signatures are not evaluated.
	 * 
	 * @param pClass the class
	 * @return all using declaratives
	 */
	public static List<String> findOperationNames(EClass pClass) {
		List<String> result = new ArrayList<String>();
		Set<String> operations = new HashSet<String>();
		if (pClass != null) {
			for (EOperation eOp : pClass.getEAllOperations()) {
				operations.add(eOp.getName());
			}			
		}
		result.addAll(operations);
		return result;
	}
}
