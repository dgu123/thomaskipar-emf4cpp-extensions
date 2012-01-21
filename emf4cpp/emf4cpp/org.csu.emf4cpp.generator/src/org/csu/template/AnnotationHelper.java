package org.csu.template;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map.Entry;

import org.eclipse.emf.common.util.EMap;
import org.eclipse.emf.ecore.EAnnotation;

/**
 * The Annotations helper contains mehtods to deals with {@link EAnnotation}.
 * 
 * <p>
 * This is necessary since the current code generator uses the JavaBeans
 * metamodel (that does not contian the "map" type for the annotations). It
 * might be a better solution to change to the emf metamodel. As long this does
 * not happen, one have to keep java code to deal with the annotations properly.
 * </p>
 * 
 * @author Thomas Kipar
 */
public class AnnotationHelper {

	/**
	 * Finds all details in a given annotation.
	 * 
	 * @param pAnnotation
	 *            the annotation to search details for.
	 * @param pKey
	 *            the key of the details to find.
	 * @return all detail values that have the given key. Never
	 *         <code>null</code> but possibly empty.
	 */
	public static List<String> findDetails(EAnnotation pAnnotation, String pKey) {
		List<String> list = new ArrayList<String>();
		if (pAnnotation != null && pKey != null) {
			EMap<String, String> details = pAnnotation.getDetails();
			for (Entry<String, String> entry : details.entrySet()) {
				if (pKey.equals(entry.getKey())) {
					list.add(entry.getValue());
				}
			}
		}
		return list;
	}

	/**
	 * Finds all included defined by a given annotation.
	 * 
	 * <p>
	 * Included are defined by the "include" key in an annotation. Multiple
	 * includes might be seperated by ";".
	 * </p>
	 * 
	 * @param pAnnotation the annotation to find the includes for.
	 * @return
	 */
	public static List<String> findIncludes(EAnnotation pAnnotation) {
		List<String> list = new ArrayList<String>();
		if (pAnnotation != null) {
			List<String> rawIncludes = findDetails(pAnnotation, "include");
			for (String rawInclude : rawIncludes) {
				list.addAll(Arrays.asList(rawInclude.split(";")));
			}
		}
		return list;
	}

}
