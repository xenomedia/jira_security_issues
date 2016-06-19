<?php

/**
 * @file
 * Code required only for the update status settings form.
 */

/**
 * Form constructor for the update settings form.
 *
 * @see jira_update_issues_settings_validate()
 * @see jira_update_issues_settings_submit()
 * @ingroup forms
 */
function jira_update_issues_settings($form) {
  $form['jira_update_issues_check_frequency'] = array(
    '#type' => 'radios',
    '#title' => t('Check for updates'),
    '#default_value' => variable_get('jira_update_issues_check_frequency', 1),
    '#options' => array(
      '1' => t('Daily'),
      '7' => t('Weekly'),
    ),
    '#description' => t('Select how frequently you want to automatically check for new releases of your currently installed modules and themes.'),
  );

  $form['jira_update_issues_level'] = array(
    '#type' => 'radios',
    '#title' => t('Security Level'),
    '#default_value' => variable_get('jira_update_issues_level', 4),
    '#options' => array(
      '1' => t('Security Only'),
      '4' => t('All Updates'),
    ),
    '#description' => t('Select how level you want to automatically check for new releases of your currently installed modules and themes.'),
  );

  $project_code = variable_get('jira_update_issues_project_code', '');
  $form['jira_update_issues_project_code'] = array(
    '#type' => 'textfield',
    '#title' => t('JIRA Project Key'),
    '#default_value' => $project_code,
    '#description' => t('The JIRA Key from your JIRA site. If your project has a code like XENO-42 then your JIRA key is XENO'),
  );

  $form = system_settings_form($form);
  // Custom validation callback to verify the JIRA Key.
  $form['#validate'][] = 'jira_update_issues_settings_validate';
  // We need to call our own submit callback first, not the one from
  // system_settings_form(), so that we can process and save the emails.
  unset($form['#submit']);

  return $form;
}

/**
 * Form validation handler for jira_update_issues_settings().
 *
 * Validates the JIRA Key exists.
 *
 * @see jira_update_issues_settings_submit()
 */
function jira_update_issues_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['jira_update_issues_project_code'])) {
    $form_state['project_code'] = $form_state['values']['jira_update_issues_project_code'];
    $project_code = $form_state['project_code'];
    $found_issues = jira_rest_project_get();
    $project_id = '';
    foreach ($found_issues as $key => $value) {
      if ($value->key == $project_code) {
        $project_id = $value->id;
        break;
      }
    }

    if (empty($project_id)) {
      form_set_error('jira_update_issues_project_code', t('%code was not found in JIRA.', array('%code' => $project_code)));
    }
  }
}

/**
 * Form submission handler for jira_update_issues_settings().
 *
 * @see jira_update_issues_settings_validate()
 */
function jira_update_issues_settings_submit($form, $form_state) {
  if (empty($form_state['project_code'])) {
    variable_del('jira_update_issues_project_code');
  }
  else {
    variable_set('jira_update_issues_project_code', $form_state['project_code']);
  }
  unset($form_state['project_code']);
  unset($form_state['values']['jira_update_issues_project_code']);

  system_settings_form_submit($form, $form_state);
}
