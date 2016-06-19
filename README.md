# JIRA Update Issues

This module is built to add module updates to your JIRA project to assign updates to a team member. This could also trigger a CI server (We are still testing Jenkins setup.)

- Requires [JIRA REST](https://www.drupal.org/project/jira_rest) and Drupal Updates enabled.

## Setup:

- Configure JIRA REST parameters admin/config/services/jira_rest
- Configure JIRA Update Issues admin/reports/jira_update_issues/settings
    - Choose daily or weekly checks (if you do not rename the JIRA ticket it will not report the same update twice)
    - Choose Security level, defaults to all releases. (does not report unsupported modules)
    - Enter your JIRA Project Key


View open tickets at admin/reports/jira_update_issues

