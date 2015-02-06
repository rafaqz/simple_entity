<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on simple_entity being loaded from the database.
 *
 * This hook is invoked during $simple_entity loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $simple_entity entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_simple_entity_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $simple_entity is inserted.
 *
 * This hook is invoked after the $simple_entity is inserted into the database.
 *
 * @param SimpleEntity $simple_entity
 *   The $simple_entity that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_simple_entity_insert(SimpleEntity $simple_entity) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('simple_entity', $simple_entity),
      'extra' => print_r($simple_entity, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $simple_entity being inserted or updated.
 *
 * This hook is invoked before the $simple_entity is saved to the database.
 *
 * @param SimpleEntity $simple_entity
 *   The $simple_entity that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_simple_entity_presave(SimpleEntity $simple_entity) {
  $simple_entity->name = 'foo';
}

/**
 * Responds to a $simple_entity being updated.
 *
 * This hook is invoked after the $simple_entity has been updated in the database.
 *
 * @param SimpleEntity $simple_entity
 *   The $simple_entity that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_simple_entity_update(SimpleEntity $simple_entity) {
  db_update('mytable')
    ->fields(array('extra' => print_r($simple_entity, TRUE)))
    ->condition('id', entity_id('simple_entity', $simple_entity))
    ->execute();
}

/**
 * Responds to $simple_entity deletion.
 *
 * This hook is invoked after the $simple_entity has been removed from the database.
 *
 * @param SimpleEntity $simple_entity
 *   The $simple_entity that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_simple_entity_delete(SimpleEntity $simple_entity) {
  db_delete('mytable')
    ->condition('pid', entity_id('simple_entity', $simple_entity))
    ->execute();
}

/**
 * Act on a simple_entity that is being assembled before rendering.
 *
 * @param $simple_entity
 *   The simple_entity entity.
 * @param $view_mode
 *   The view mode the simple_entity is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $simple_entity->content prior to rendering. The
 * structure of $simple_entity->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_simple_entity_view($simple_entity, $view_mode, $langcode) {
  $simple_entity->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for simple_entitys.
 *
 * @param $build
 *   A renderable array representing the simple_entity content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * simple_entity content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the simple_entity rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_simple_entity().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_simple_entity_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on simple_entity_type being loaded from the database.
 *
 * This hook is invoked during simple_entity_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of simple_entity_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_simple_entity_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a simple_entity_type is inserted.
 *
 * This hook is invoked after the simple_entity_type is inserted into the database.
 *
 * @param SimpleEntityType $simple_entity_type
 *   The simple_entity_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_simple_entity_type_insert(SimpleEntityType $simple_entity_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('simple_entity_type', $simple_entity_type),
      'extra' => print_r($simple_entity_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a simple_entity_type being inserted or updated.
 *
 * This hook is invoked before the simple_entity_type is saved to the database.
 *
 * @param SimpleEntityType $simple_entity_type
 *   The simple_entity_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_simple_entity_type_presave(SimpleEntityType $simple_entity_type) {
  $simple_entity_type->name = 'foo';
}

/**
 * Responds to a simple_entity_type being updated.
 *
 * This hook is invoked after the simple_entity_type has been updated in the database.
 *
 * @param SimpleEntityType $simple_entity_type
 *   The simple_entity_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_simple_entity_type_update(SimpleEntityType $simple_entity_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($simple_entity_type, TRUE)))
    ->condition('id', entity_id('simple_entity_type', $simple_entity_type))
    ->execute();
}

/**
 * Responds to simple_entity_type deletion.
 *
 * This hook is invoked after the simple_entity_type has been removed from the database.
 *
 * @param SimpleEntityType $simple_entity_type
 *   The simple_entity_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_simple_entity_type_delete(SimpleEntityType $simple_entity_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('simple_entity_type', $simple_entity_type))
    ->execute();
}

/**
 * Define default simple_entity_type configurations.
 *
 * @return
 *   An array of default simple_entity_type, keyed by machine names.
 *
 * @see hook_default_simple_entity_type_alter()
 */
function hook_default_simple_entity_type() {
  $defaults['main'] = entity_create('simple_entity_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default simple_entity_type configurations.
 *
 * @param array $defaults
 *   An array of default simple_entity_type, keyed by machine names.
 *
 * @see hook_default_simple_entity_type()
 */
function hook_default_simple_entity_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
