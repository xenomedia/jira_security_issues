<?php

/**
 * @file
 * Code required only when rendering the available updates report.
 */
function issetor(&$var, $default = false) {
  return isset($var) ? $var : $default;
}
/**
 * Page callback: Generates a page about the update status of projects.
 *
 * @see jira_update_issues_menu()
 */
function jira_update_issues_status() {
  $project_code = variable_get('jira_update_issues_project_code', '');
  $found_issues = jira_rest_issue_search('project = "' . $project_code . '" and resolution = Unresolved ORDER BY updatedDate ASC');
  $header = array('Key', 'Status', 'Summary', 'Description', 'Reporter', 'Assignee');
  $rows = array();
  foreach ($found_issues as $issue){
    $rows[] = array($issue->key,
      $issue->fields->status->name,
      $issue->fields->summary,
      $issue->fields->description,
      $issue->fields->reporter->displayName,
      issetor($issue->fields->assignee->displayName, 'unassigned'),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}
