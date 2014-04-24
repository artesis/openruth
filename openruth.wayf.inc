<?php
/**
 * @file
 * Implements functions used to log users into the system through WAYF.
 */

/**
 * Implements hook_wayf_login_credentials().
 *
 * Used to set the name and password to log into the library system after WAYF
 * login.
 */
function openruth_wayf_login_credentials($authentication_attributes) {
  $hash = variable_get('openruth_wayf_hash', NULL);

  // Set message that hash value was not found.
  if (!$hash) {
    watchdog('openruth', t('The WAYF hash value has not been set in setting.php as $conf[\'openruth_wayf_hash\'] and WAYF login will fail'), WATCHDOG_ERROR);

    return array();
  }

  $cpr = module_invoke(
    'ding_ucfmd',
    'retrieve_cpr',
    $authentication_attributes['organizationName'],
    $authentication_attributes['eduPersonPrincipalName']
  );

  // Set message that CPR value was not found.
  if (!$cpr) {
    watchdog(
      'openruth',
      t('The CPR value has not been retrieved from UCFMD with attributes <pre>!t</pre>'),
      array('!t' => var_export($authentication_attributesm, true)),
      WATCHDOG_ERROR
    );

    return array();
  }

  return array(
    'name' => $cpr,
    'pass' => $hash,
  );
}

/**
 * Implements hook_wayf_attributes().
 *
 * Used to define which attribute to use as login name.
 */
function openruth_wayf_attributes() {
  return array(
    'organizationName' => array(
      'field' => NULL,
    ),
    'eduPersonPrincipalName' => array(
      'field' => NULL,
    ),
  );
}
