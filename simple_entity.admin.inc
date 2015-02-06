<?php

/**
 * Generates the simple_entity type editing form.
 */
function simple_entity_type_form($form, &$form_state, $simple_entity_type, $op = 'edit') {

  if ($op == 'clone') {
    $simple_entity_type->label .= ' (cloned)';
    $simple_entity_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $simple_entity_type->label,
    '#description' => t('The human-readable name of this simple_entity type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($simple_entity_type->type) ? $simple_entity_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $simple_entity_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'simple_entity_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this simple_entity type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($simple_entity_type->description) ? $simple_entity_type->description : '',
    '#description' => t('Description about the simple_entity type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save simple_entity type'),
    '#weight' => 40,
  );

  if (!$simple_entity_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete simple_entity type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('simple_entity_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing simple_entity_type.
 */
function simple_entity_type_form_submit(&$form, &$form_state) {
  $simple_entity_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  simple_entity_type_save($simple_entity_type);

  // Redirect user back to list of simple_entity types.
  $form_state['redirect'] = 'admin/structure/simple_entity-types';
}

function simple_entity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/simple_entity-types/' . $form_state['simple_entity_type']->type . '/delete';
}

/**
 * Simple Entity type delete form.
 */
function simple_entity_type_form_delete_confirm($form, &$form_state, $simple_entity_type) {
  $form_state['simple_entity_type'] = $simple_entity_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['simple_entity_type_id'] = array('#type' => 'value', '#value' => entity_id('simple_entity_type' ,$simple_entity_type));
  return confirm_form($form,
    t('Are you sure you want to delete simple_entity type %title?', array('%title' => entity_label('simple_entity_type', $simple_entity_type))),
    'simple_entity/' . entity_id('simple_entity_type' ,$simple_entity_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Simple Entity type delete form submit handler.
 */
function simple_entity_type_form_delete_confirm_submit($form, &$form_state) {
  $simple_entity_type = $form_state['simple_entity_type'];
  simple_entity_type_delete($simple_entity_type);

  watchdog('simple_entity_type', '@type: deleted %title.', array('@type' => $simple_entity_type->type, '%title' => $simple_entity_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $simple_entity_type->type, '%title' => $simple_entity_type->label)));

  $form_state['redirect'] = 'admin/structure/simple_entity-types';
}

/**
 * Page to select simple_entity Type to add new simple_entity.
 */
function simple_entity_admin_add_page() {
  $items = array();
  foreach (simple_entity_types() as $simple_entity_type_key => $simple_entity_type) {
    $items[] = l(entity_label('simple_entity_type', $simple_entity_type), 'simple_entity/add/' . $simple_entity_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of simple_entity to create.')));
}

/**
 * Add new simple_entity page callback.
 */
function simple_entity_add($type) {
  $simple_entity_type = simple_entity_types($type);

  $simple_entity = entity_create('simple_entity', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('simple_entity_type', $simple_entity_type))));

  $output = drupal_get_form('simple_entity_form', $simple_entity);

  return $output;
}

/**
 * Simple Entity Form.
 */
function simple_entity_form($form, &$form_state, $simple_entity) {
  $form_state['simple_entity'] = $simple_entity;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $simple_entity->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $simple_entity->description,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $simple_entity->uid,
  );

  field_attach_form('simple_entity', $simple_entity, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save ') . $simple_entity->type,
    '#submit' => $submit + array('simple_entity_form_submit'),
  );

  // Show Delete button if we edit simple_entity.
  $simple_entity_id = entity_id('simple_entity' ,$simple_entity);
  if (!empty($simple_entity_id) && simple_entity_access('edit', $simple_entity)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('simple_entity_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'simple_entity_form_validate';

  return $form;
}

function simple_entity_form_validate($form, &$form_state) {

}

/**
 * Simple Entity submit handler.
 */
function simple_entity_form_submit($form, &$form_state) {
  $simple_entity = $form_state['simple_entity'];

  entity_form_submit_build_entity('simple_entity', $simple_entity, $form, $form_state);

  simple_entity_save($simple_entity);

  $simple_entity_uri = entity_uri('simple_entity', $simple_entity);

  $form_state['redirect'] = $simple_entity_uri['path'];

  drupal_set_message(t('Simple Entity %title saved.', array('%title' => entity_label('simple_entity', $simple_entity))));
}

function simple_entity_form_submit_delete($form, &$form_state) {
  $simple_entity = $form_state['simple_entity'];
  $simple_entity_uri = entity_uri('simple_entity', $simple_entity);
  $form_state['redirect'] = $simple_entity_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function simple_entity_delete_form($form, &$form_state, $simple_entity) {
  $form_state['simple_entity'] = $simple_entity;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['simple_entity_type_id'] = array('#type' => 'value', '#value' => entity_id('simple_entity' ,$simple_entity));
  $simple_entity_uri = entity_uri('simple_entity', $simple_entity);
  return confirm_form($form,
    t('Are you sure you want to delete simple_entity %title?', array('%title' => entity_label('simple_entity', $simple_entity))),
    $simple_entity_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function simple_entity_delete_form_submit($form, &$form_state) {
  $simple_entity = $form_state['simple_entity'];
  simple_entity_delete($simple_entity);

  drupal_set_message(t('Simple Entity %title deleted.', array('%title' => entity_label('simple_entity', $simple_entity))));

  $form_state['redirect'] = '<front>';
}
