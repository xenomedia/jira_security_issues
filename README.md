# JIRA Security Issues

This module is built to add module updates to your JIRA project to assign updates to a team member. This could also trigger a CI server (We are still testing Jenkins setup.)

- Requires [JIRA REST](https://www.drupal.org/project/jira_rest)

## Setup:


- Configure JIRA REST parameters admin/config/services/jira_rest
- Configure JIRA Security Issues admin/reports/jira_security_issues/settings
    - Choose daily or weekly checks (if you do not rename the JIRA ticket it will not report the same update twice)
    - Enter your JIRA Project Key


View open tickets at admin/reports/jira_security_issues
