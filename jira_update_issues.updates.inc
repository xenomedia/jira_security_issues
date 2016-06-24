<?php
/**
 * Created by PhpStorm.
 * User: michaelpporter
 * Date: 5/26/16
 * Time: 1:08 PM
 */


function jira_update_issues_send_to_jira() {
  $now = time();
  variable_set('jira_update_issues_last_check', $now);

  if ($available = update_get_available(TRUE)) {
    module_load_include('inc', 'update', 'update.compare');
    $data = update_calculate_project_data($available);

    $count = 0;
    $project_code = variable_get('jira_update_issues_project_code', '');
    $foundissues = jira_rest_project_get();
    foreach ($foundissues as $key => $value) {
      if ($value->key == $project_code) {
        $project_id = $value->id;
        break;
      }
    }

    foreach ($data as $key => $value) {
      if (isset($value['info']['version']) && !empty($value['info']['version'])
        && isset($value['recommended']) && !empty($value['recommended'])
        && $value['info']['version'] <> $value['recommended']
      ) {
        $security_level = variable_get('jira_update_issues_level', 4);
        if ($value['status'] == 1
          || ($value['status'] == 4
            && $security_level == 4)
        ) {
          $summary = 'Update ' . $value['name'] . ' to ' . $value['recommended'];
          $issuetypeid = 2;
          if ($value['status'] == 1) {
            $summary = 'SECURITY ' . $summary;
            $issuetypeid = 1;
          }
          if ($value['status'] == 4) {
            $summary = 'RELEASE ' . $summary;
            $issuetypeid = 2;
          }


          $foundissues = jira_rest_issue_search('summary ~  "' . $summary .
            '" AND project = ' . $project_code . ' ORDER BY updatedDate DESC');
          if (!count($foundissues)) {
            $issuedata =
              array(
                'fields' => array(
                  'project' => array('id' => $project_id,),
                  'summary' => $summary,
                  'description' => $summary,
                  'priority' => array('id' => (string) $value['status'],),
                  'issuetype' => array('id' => (string) $issuetypeid,),
                  'labels' => array($value['name']),
                ),
              );

            $jira_case = jira_rest_issue_create($issuedata);
          }
          $mod_list[$count]['name'] = $value['name'];
          $mod_list[$count]['version'] = $value['info']['version'];
          $mod_list[$count]['project'] = $value['info']['project'];
          $mod_list[$count]['package'] = $value['info']['package'];
          $mod_list[$count]['project_type'] = $value['project_type'];
          $mod_list[$count]['link'] = $value['link'];
          $mod_list[$count]['status'] = $value['status'];
          $mod_list[$count]['recommended'] = $value['recommended'];
          $mod_list[$count]['latest_version'] = $value['latest_version'];
          $mod_list[$count]['project_status'] = $value['project_status'];
          $mod_list[$count]['siteversion'] = DRUPAL_CORE_COMPATIBILITY;

          $count = $count + 1;
        }

      }
    }

  }

  //print_r(json_encode($mod_list));
  return json_encode($mod_list);
}
