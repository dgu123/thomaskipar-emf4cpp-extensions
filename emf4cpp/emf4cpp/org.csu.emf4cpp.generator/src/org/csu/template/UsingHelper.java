package org.csu.template;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EOperation;
import org.eclipse.emf.ecore.EPackage;

/**
 * Utility class to create the using statements.
 * 
 * @author Thomas Kipar
 */
public class UsingHelper {

	/**
	 * For a given class, this method will return the full qualified names of
	 * all operations to use for the using declaratives.
	 * 
	 * @param pClass
	 *            the class
	 * @return all using declaratives
	 */
	public static List<String> findUsingOperations(EClass pClass) {
		List<String> result = new ArrayList<String>();
		Set<String> operations = new HashSet<String>();
		Set<EOperation> addedOps = new HashSet<EOperation>();

		if (pClass != null) {
			for (EOperation eOp : pClass.getEAllOperations()) {
				if (eOp.getEContainingClass() != pClass) {
					boolean opWithSigAdded = false;
					for (EOperation addedOp : addedOps) {
						if (addedOp.getName().equals(eOp.getName())
								&& addedOp.getEParameters().size() == eOp
										.getEParameters().size()) {
							boolean parametersMatch = true;
							for (int i = 0; i < addedOp.getEParameters().size(); ++i) {
								parametersMatch &= addedOp.getEParameters()
										.get(i).getEType() == eOp
										.getEParameters().get(i).getEType()
										&& addedOp.getEParameters().get(i)
												.isMany() == eOp
												.getEParameters().get(i)
												.isMany();
							}
							opWithSigAdded |= parametersMatch;
						}
					}

					if (!opWithSigAdded) {
						operations.add(toFQN(eOp));
						addedOps.add(eOp);
					}

				}
			}
		}
		result.addAll(operations);
		return result;
	}

	private static String toFQN(EOperation pOp) {
		StringBuilder builder = new StringBuilder();

		EPackage p = pOp.getEContainingClass().getEPackage();
		while (p != null) {
			builder.insert(0, "::" + p.getName());
			p = p.getESuperPackage();
		}
		builder.append("::").append(pOp.getEContainingClass().getName());
		builder.append("::").append(pOp.getName());

		return builder.toString();
	}
}
