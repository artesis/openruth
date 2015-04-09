<?php

/**
 * Get list of user loans.
 */
function openruth_loan_list($account) {
  $creds = ding_user_get_creds($account);
  $result = array();
  $status = _openruth_user_status($creds);

  if (isset($status->loans->loan)) {
    foreach ($status->loans->loan as $loan) {
      $result[$loan->copyId] = new DingProviderLoan($loan->copyId, array(
        'ding_entity_id' => $loan->itemId,
        'display_name' => $loan->itemDisplayTitle,
        'loan_date' => $loan->loanDate,
        'expiry' => $loan->loanReturnDate,
        'renewable' => $loan->loanRenewable == 'renewable',
        'materials_number' => $loan->copyId,
        'author' => !empty($loan->itemAuthor) ? $loan->itemAuthor : '',
        'title' => !empty($loan->itemTitle) ? $loan->itemTitle : '',
        'publication_year' => !empty($loan->itemPublicationYear) ? $loan->itemPublicationYear : '',
      ));
    }
  }

  return $result;
}

/**
 * Get a list of ILL loans.
 *
 * @param object $account
 *   Current logged account, whose loans are to be fetched.
 * @return DingProviderLoan
 *   Array of objects, representing a collection of ILL's.
 */
function openruth_ill_list($account) {
  $creds = ding_user_get_creds($account);
  $result = array();
  $status = _openruth_user_status($creds);

  if (isset($status->illOrders->illOrder)) {
    foreach ($status->illOrders->illOrder as $illOrder) {
      $result[$illOrder->orderId] = new DingProviderLoan($illOrder->orderId, array(
        'ding_entity_id' => $illOrder->itemId,
        'display_name' => $illOrder->itemDisplayTitle,
        'loan_date' => $illOrder->orderDate,
        'expiry' => $illOrder->orderLastInterestDate,
        'renewable' => FALSE,
        'materials_number' => $illOrder->orderId,
        'author' => !empty($illOrder->itemAuthor) ? $illOrder->itemAuthor : '',
        'title' => $illOrder->itemTitle,
        'publication_year' => !empty($illOrder->itemPublicationYear) ? $illOrder->itemPublicationYear : '',
      ));
    }
  }

  return $result;
}

/**
 * Renew loans.
 */
function openruth_loan_renew($account, $ids) {
  $creds = ding_user_get_creds($account);

  $result = array();
  $res = openruth_client_invoke('renew_loan', $creds['name'], $ids);

  if ($res == TRUE) {
    $status = _openruth_user_status($creds, TRUE);
  }
  foreach ($ids as $id) {
    if ($res[$id] === TRUE) {
      $result[$id] = DingProviderLoan::STATUS_RENEWED;
    }
    else {
      $result[$id] = DingProviderLoan::STATUS_NOT_RENEWED;
    }
  }
  return $result;
}
